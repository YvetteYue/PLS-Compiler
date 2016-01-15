#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
void Condition()        //判断条件
{
    Item x=expr();
    if(sym==LSS||sym==LEQ||sym==NEQ||sym==GTR||sym==GEQ||sym==EQL)
    {
        int op=sym;
        getSym();
        Item y=expr();
        int type=resultType(x.typ,y.typ);
        if(type==intt) //类型问题
        {
            switch(op)
            {
            case EQL:
                emit(ir_EQL);
                break;
            case NEQ:
                emit(ir_NEQ);
                break;
            case LSS:
                emit(ir_LSS);
                break;
            case GTR:
                emit(ir_GRT);
                break;
            case GEQ:
                emit(ir_GEQ);
                break;
            case LEQ:
                emit(ir_LEQ);
                break;
            }
        }
        else if(type==real)
        {
            switch(op)
            {
            case EQL:
                emit(ir_EQR);
                break;
            case NEQ:
                emit(ir_NER);
                break;
            case LSS:
                emit(ir_LSR);
                break;
            case GTR:
                emit(ir_GTR);
                break;
            case GEQ:
                emit(ir_GER);
                break;
            case LEQ:
                emit(ir_LER);
                break;
            }
        }
        else ParserError("表达式类型错误");
    }
    else ParserError("未知的比较运算符");
}
void ConditionStm()     //条件语句
{
    int lc1,lc2;
    if(sym==IF)
    {
        getSym();
        Condition();
        lc1=lc; //代码入口1
        emit(ir_JPC);//JPC条件转移
        if(sym==THEN)
        {
            getSym();
            statement();
            if(sym==ELSE)
            {
                getSym();
                lc2=lc;//代码入口2
                emit(ir_JMP);//无条件转移
                code[lc1].y=lc;//LabA
                statement();
                code[lc2].y=lc;//LabB
            }
            else code[lc1].y=lc; //LabA
        }
        else ParserError("if stm no  then");
    }
    else ParserError("不是if语句");
    //  cout<<"This is a conditional statement."<<endl;
}
void WhileStm()     //循环语句
{
    int lc1,lc2;
    if(sym==WHILE)
    {
        getSym();
        lc1=lc;
        Condition();
        lc2=lc;
        emit(ir_JPC);   //JPC条件转移
        if(sym==DO)
        {
            getSym();
            statement();
            emit1(ir_JMP,lc1);  //无条件转移到循环循环初始位置
            code[lc2].y=lc;
        }
        else ParserError("while语句缺少do关键字。。");
    }
    else ParserError("undefine");
    //  cout<<"This is a while-statement."<<endl;
}
void ForStm()       //for循环语句
{
    int cvt,i,f,lc1,lc2;
    Item x;
    if(sym==FOR)
    {
        getSym();
        if(sym==IDEN)
        {
            i=loc(id);//符号表中查找标识符类型
            getSym();
            if(i==0)
                cvt=intt;
            else if(tab[i].obj==variable)    //变量类型
            {
                cvt=tab[i].typ;
                if(!tab[i].normal)ParserError("not value var");
                else emit2(ir_LDA,tab[i].lev,tab[i].adr);    //LDA指令类型
            }
            else ParserError("for语句循环变量类型错误,不是变量");
            if(sym==ASSIGN)
            {
                getSym();
                x=expr();
                if(x.typ!=cvt)
                    ParserError("循环变量后的初值类型和该循环变量类型不统一");
                if(sym==DOWNTO||sym==TO)
                {
                    if(sym==DOWNTO)
                        f=ir_F1D;   //F1D
                    else f=ir_F1U;   //F1U
                    getSym();
                    x=expr();
                    if(x.typ!=cvt)
                        ParserError("终值与循环变量不统一");
                    lc1=lc;     //代码入口1
                    emit(f);    //入口判断
                    if(sym==DO)
                    {
                        getSym();
                        lc2=lc;     //代码入口2
                        statement();
                        emit1(f+1,lc2);     //回到循环初始位置 F2U或F2D 在入口判断
                        code[lc1].y=lc;     //退出循环
                    }
                    else ParserError("for语句没有do关键字");
                }
                else ParserError("没有to或downto关键字");
            }
            else ParserError("循环头部没有赋值语句");
        }
        else ParserError("for语句中没有循环变量");
    }
    else ParserError("不是for语句");
    // cout<<"This is a for-statement."<<endl;
}
void WriteStm()             //写语句
{
    int i,f;
    Item x,y;
    if(sym==WRITE)
    {
        getSym();
        if(sym==LPARENT)
        {
            getSym();
            if(sym==STRINGCON)      //字符串
            {
                //emit1(24,strlen(id));
                emit1(ir_WRS,sx);//TODO
                sx++;
                getSym();
                if(sym==COMMA)
                {
                    getSym();
                    x=expr();   //表达式求值
                    emit1(ir_WRW,x.typ);
                }
            }
            else
            {
                x=expr();
                emit1(ir_WRW,x.typ);
            }
            if(sym==RPARENT)
            {
                getSym();
                emit(ir_WRL);
            }
            else ParserError("no )");
        }
        else ParserError("no (");
    }
    else ParserError("undefine type");
    //  cout<<"This is a write statement."<<endl;
}
void ReadStm()
{
    int i,f;
    Item x,y;
    if(sym==READ)
    {
        getSym();
        if(sym==LPARENT)
        {
            do
            {
                getSym();
                if(sym!=IDEN)ParserError("read iden not iden");
                else
                {
                    i=loc(id);
                    getSym();
                    if(i!=0)
                    {
                        if(tab[i].obj!=variable)
                            ParserError("read iden not var");
                        else
                        {
                            x.typ=tab[i].typ;
                            x.reff=tab[i].reff;
                            if(tab[i].normal)
                                f=ir_LDA;
                            else f=ir_LOD;
                            emit2(f,tab[i].lev,tab[i].adr);
                            emit1(ir_RED,x.typ);
                        }
                    }
                }
            }
            while(sym==COMMA);
            if(sym==RPARENT)
            {
                getSym();
            }
            else ParserError("no )");
        }
    }
    // cout<<"This is a read statement."<<endl;
}
/*
    赋值语句：参数1：标识符所在层，变量所在相对地址，标识符所在位置
    <赋值语句>::=<标识符>:=<表达式>|<函数标识符>:=<表达式>|<标识符>'['<表达式>']':=<表达式>
*/
void assignment(int lv,int ad,int i)
{
    Item x,y;
    int f;
    x.typ=tab[i].typ;           //标识符类型
    x.reff=tab[i].reff;         //标识符指向对应符号表中的位置
    if(tab[i].normal)       //变量形参，值形参
        f=ir_LDA;
    else f=ir_LOD;
    emit2(f,lv,ad);
    if(sym==LSQUARE) //数组
    {
        x=CallArray(x);
    }
    if(sym==ASSIGN)
    {
        getSym();
    }
    else ParserError("没有赋值符号");
    y=expr();
    if(x.typ==charr)
    {
        x.typ=intt;
    }
    if(y.typ==charr)
    {
        y.typ=intt;
    }
    if(x.typ==y.typ)
    {
        emit(ir_STO);
    }
    else if(x.typ==real&&y.typ==intt)
    {
        emit1(ir_FLT,0);
        emit(ir_STO);
    }
    else if(x.typ==notyp&&y.typ==notyp)
    {
        ParserError("assign type error");
    }
    else ParserError("有错");
    // cout<<"赋值语句"<<endl;
}
