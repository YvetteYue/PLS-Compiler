#include "header.h"     //DONE
#include "def.h"
#include<iostream>
using namespace std;
/*
    Conrec Const();
    参数：无
    返回值：Conrec: tp->常量类型    i->字符或整数值   r->实数值
    函数功能:
    <常量>::=[+|-](<无符号整数>|<无符号实数>)|<字符>
*/
Conrec Const()
{
    Conrec c;
    int x,sign=1;
    if(sym==CHARCON)    //字符
    {
        c.tp=charr;
        c.i=id[1];
        getSym();
    }
    else
    {
        if(sym==PLUS||sym==MINUS)
        {
            if(sym==MINUS)
            {
                sign=-1;
            }
            getSym();
        }
        if(sym==INTCON)     //整数
        {
            getSym();
            c.tp=intt;
            c.i=sign*atoi(id);
        }
        else if(sym==REALCON)   //实数
        {
            getSym();
            c.tp=real;
            c.r=sign*atof(id);
        }
        else Error("const not int,char,real");
    }
    //cout<<"This is a constant."<<endl;
    return c;
}

/*
    void ConstDefine();
    参数：无
    函数功能：常量定义
    <常量定义>::=<标识符>=<常量>
*/
void ConstDefine()
{
    if(sym==IDEN)
    {
        enter(id,constant); //存常量
        getSym();
        if(sym==EQL)
        {
            getSym();
            Conrec c=Const();     //常量值
            tab[t].typ=c.tp;
            tab[t].reff=0;
            if(c.tp==real)
            {
                enterreal(c.r);     //存实数
                tab[t].adr=c1;      //tab中存索引
            }
            else tab[t].adr=c.i;
        }
        else ParserError("常量定义没有'='");
    }
    else ParserError("");
    // cout<<"This is a constant define."<<endl;
}

/*
    void ConstDescripSec();
    参数：无
    函数功能：常量说明部分：
    <常量说明部分>::=const<常量定义>{,<常量定义>};
*/
void ConstDescripSec()
{
    if(sym==CONST)
    {
        getSym();
        ConstDefine();
        while(sym==COMMA)
        {
            getSym();
            ConstDefine();
        }
        if(sym==SEMI)
        {
            getSym();
        }
        else ParserError("const define no ;");
    }
    //  cout<<" This is constant description section."<<endl;
}

/*
    void type(int t0,int t1);
    参数: t0  标识符在tab中的起始位置  针对的是多标识符定义
          t1  结束位置
    函数功能：
    //<类型>::=<基本类型>|array'['<无符号整数>']' of <基本类型>
    //<基本类型>::=integer|char|real
*/
void type(int t0,int t1)
{
    int tp,sz;
    tp=notyp;
    if(sym==CHAR||sym==REAL||sym==INT)          //基本类型
    {
        if(sym==CHAR)tp=charr;
        else if(sym==REAL) tp=real;
        else if(sym==INT)tp=intt;
        getSym();
        enterVar(t0,t1,tp,true);
    }
    else if(sym==ARRAY)             //数组类型
    {
        getSym();
        if(sym==LSQUARE)
        {
            getSym();
            if(sym==INTCON)         //下标类型：无符号整数
            {
                sz=atoi(id);        //数组长度
                getSym();
                if(sym==RSQUARE)
                {
                    getSym();
                    if(sym==OF)
                    {
                        getSym();
                        if(sym==CHAR||sym==INT||sym==REAL)
                        {
                            if(sym==CHAR)tp=charr;
                            else if(sym==REAL) tp=real;
                            else if(sym==INT)tp=intt;
                            enterArray(t0,t1,sz,tp);    //标识符开始tab中位置，结束位置，数组长度，数组类型
                            getSym();
                        }
                        else ParserError("array define not char,int,real");
                    }
                    else ParserError("array define no 'of'");
                }
                else ParserError("array define no ')'");
            }
            else ParserError("array index not int type");
        }
        else ParserError("array define no ')'");
    }
    else ParserError("undefine");
    //  cout<<"This is type\n";
}

/*
    void VarDesc();
    函数声明：
    <变量说明>::=<标识符>{,<标识符>}:<类型>
*/
void VarDesc()      //
{
    int t0,t1;
    if(sym==IDEN)
    {
        t0=t;//初值
        enter(id,variable);
        getSym();
        while(sym==COMMA)
        {
            getSym();
            if(sym==IDEN)
            {
                enter(id,variable);     //登录符号表
                getSym();
            }
            else ParserError("no iden after , ");
        }
        if(sym==COLON)
        {
            t1=t;//结束值
            getSym();
        }
        else ParserError("no :");
        type(t0,t1);
    }
    else ParserError("undefine");
    //  cout<<"This is variable description."<<endl;
}

/*
    void VarDescripSec();
    函数说明：
    <变量说明部分>::=var<变量说明>;{<变量说明>;}

*/
void VarDescripSec()
{
    if(sym==VAR)
    {
        getSym();
        VarDesc();
        if(sym==SEMI)
        {
            getSym();
        }
        while(sym==IDEN)
        {
            VarDesc();
            if(sym==SEMI)
            {
                getSym();
            }
            else ParserError("miss ;");
        }
    }
    else ParserError("undefine");
    //  cout<<"This is variable description."<<endl;
}
