#include "header.h"     //TODO
#include "def.h"
#include<iostream>
using namespace std;
void ParserError(string str)
{
    Error(str);
}
/*
    void statement();
    函数功能:
    <语句>::=<赋值语句>|<条件语句>|<当循环语句>|
        <过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
*/
void statement()
{
    int i;
    switch(sym)
    {
    case IF:
        ConditionStm();
        break;
    case WHILE:
        WhileStm();
        break;
    case FOR:
        ForStm();
        break;
    case READ:
        ReadStm();
        break;
    case WRITE:
        WriteStm();
        break;
    case BEGIN:
        CompondStm();
        break;
    case IDEN:          //语句开始为标识符
    {
        i=loc(id);      //查找该标识符在符号表中的位置
        getSym();
        if(i!=0)
        {
            switch(tab[i].obj)
            {
            case constant:      //常量不能再语句的最左，即不能作为变量名
                ParserError("常量不能在语句的最左");
                break;
            case variable:      //语句开始为变量，说明函数为赋值语句
                assignment(tab[i].lev,tab[i].adr,i);    //标识符所在层，变量所在相对地址，标识符所在位置
                break;
            case procedure:     //如果标识符为过程，说明是调用过程
                if(tab[i].lev!=0)
                    ActualPara(i);      //实参
                break;
            case functionn:     //如果标识符为函数，说明是函数返回值赋值，即是赋值语句
                if(tab[i].reff==display[level])     //如果该函数名指向的分程序和当前函数分程序一致，说明函数返回正确
                    assignment(tab[i].lev+1,0,i);     //当前层次的内层,变量所在相对地址，标识符所在位置。
                else  Error("返回语句不合法，非该函数的返回语句");
                break;
            }
        }
        else ParserError("未知标识符");
        break;              //语句为空就结束
    }
    }
    // cout<<"This is a statements.\n";
}
void CompondStm()               //复合语句：<复合语句>:=begin<语句>{;<语句>}end
{
    if(sym==BEGIN)
    {
        getSym();
        statement();
        while(sym==SEMI)
        {
            getSym();
            statement();
        }
        if(sym==END)
        {
            getSym();
        }
        else ParserError("未知标识符(缺分号)");
    }
    else ParserError("没有begin开始符");
    // cout<<"This is a compond statement\n";
}
void Block()    //模块函数  <程序>::=<分程序>::=[<常量说明部分>][<变量说明部分>]{[<过程说明部分>]|[<函数说明部分>]}<复合语句>
{
    int prt,prb,x;
    dx=5;
    prt=t;                  //取当前符号表位置
    enter("  ",procedure);
    enterblock();           //将该函数或过程名存入符号表中
    prb=b;                  //取当前分程序表位置
    display[0]=b;        //当前level的分程序索引为b
    tab[prt].typ=notyp;      //将该函数的类型为notyp
    tab[prt].reff=prb;          //符号表的reff索引指向btab分程序索引位置。
    btab[prb].lastpara=t;        //设置该函数或过程的最后一个参数在符号表中的位置
    btab[prb].psize=dx;         //内务信息区+参数声明
    if(sym==CONST)
    {
        ConstDescripSec();
    }
    if(sym==VAR)
    {
        VarDescripSec();
    }
    btab[prb].vsize=dx;         //内务信息区+参数声明+变量
    while(sym==PROCSY||sym==FUNSY)
    {
        proce();
    }
    if(sym==BEGIN)
    {
        inProgram=prb;      //程序入口的符号表位置
        tab[prb].adr=lc;
        CompondStm();
    }
    else ParserError("no 'begin'");
    if(sym==POT)
    {
        emit(ir_HLT);                   //程序结束标志
    }
    else ParserError("缺少程序结束符'.'");

    //  cout<<"Block"<<endl;
}
void SubBlock(bool isfun)       //函数和过程说明部分
{
    int prt,prb,x;
    level++;                //进入内层，函数参数属于内层
    dx=5;
    prt=t;                  //取当前符号表位置
    enterblock();           //将该函数或过程名存入符号表中
    prb=b;                  //取当前分程序表位置
    display[level]=b;        //当前level的分程序索引为b
    tab[prt].typ=notyp;      //将该函数的类型为notyp
    tab[prt].reff=prb;          //符号表的reff索引指向btab分程序索引位置。
    if(sym==LPARENT) FormalParaTable();  //如果取得当前为左括号,进入形式参数函数
    btab[prb].lastpara=t;        //设置该函数或过程的最后一个参数在符号表中的位置
    btab[prb].psize=dx;         //内务信息区
    if(isfun)                   //是函数的情况
    {
        if(sym==COLON)          //符号为:
        {
            getSym();
            if(sym==INT) tab[prt].typ=intt; //函数返回值类型
            else if(sym==CHAR) tab[prt].typ=charr;
            else if(sym==REAL) tab[prt].typ=real;
            else ParserError("函数返回类型错误");
            getSym();
        }
        else ParserError("函数声明没有返回值");
    }
    if(sym==SEMI) getSym();
    else ParserError("no ;");
    if(sym==CONST)
    {
        ConstDescripSec();
    }
    if(sym==VAR)
    {
        VarDescripSec();
    }
    btab[prb].vsize=dx;
    while(sym==PROCSY||sym==FUNSY)
    {
        proce();
    }
    tab[prt].adr=lc;        //函数或过程的地址为目标代码的入口地址
    if(sym==BEGIN)
    {
        CompondStm();
    }
    else ParserError("没有符合语句");
    //  cout<<"This is a subblock."<<endl;
    level--;            //退出该层
}
