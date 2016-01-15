#include "header.h"     //TODO
#include "def.h"
#include<iostream>
using namespace std;
/*
    void ActualPara(int i);
    函数功能:
    <实在参数表>::='('<实在参数表>{,<实在参数表>}')'
*/
void ActualPara(int i)
{
    int lastp,cp,k;
    Item x;
    emit1(ir_MKS,i);        //标记栈
    lastp=btab[tab[i].reff].lastpara;//分程序表中参数的最后一个参数类型
    cp=i;//符号表中函数位置
    if(sym==LPARENT) //（
    {
        do
        {
            getSym();
            if(cp>=lastp) ParserError("传入参数个数错误");//如果函数名在符号表位置比一个形参所在位置大那么报错
            else
            {
                cp++;
                if(tab[cp].normal) //值形参
                {
                    x=expr();   //取参数类型
                    if(x.typ==tab[cp].typ)  //如果表达式类型和形参类型相同
                    {
                        if(x.reff!=tab[cp].reff)
                        {
                            ParserError("形参与实参类型不同");
                        }
                    }
                    else if(x.typ==intt&&tab[cp].typ==real)
                    {
                        emit1(ir_FLT,0);    //当前值转换成浮点数
                    }
                    else if(x.typ==charr&&tab[cp].typ==intt)
                    {
                        x.typ=intt;
                    }
                    else
                    {
                        ParserError("error para typ");
                    }
                }
                else   //变量形参
                {
                    if(sym!=IDEN)ParserError("no para");
                    else
                    {
                        k=loc(id);      //查找标识符所在位置
                        getSym();//
                        if(k!=0)
                        {
                            if(tab[k].obj!=variable) ParserError("not var");
                            x.typ=tab[k].typ;       //类型
                            x.reff=tab[k].reff;      //指向atab或btab
                            if(tab[k].normal)        //是否为值形参
                            {
                                emit2(ir_LDA,tab[k].lev,tab[k].adr);     //取实际参数的地址
                            }
                            else emit2(ir_LOD,tab[k].lev,tab[k].adr);    //取实际参数的值
                            if(sym==LSQUARE)    //传入参数为数组
                            {
                                x=CallArray(x);
                            }
                            if(x.typ==charr&&tab[cp].typ==intt)
                            {
                                x.typ=intt;
                            }
                            if(x.typ!=tab[cp].typ||x.reff!=tab[cp].reff)     //
                            {
                                ParserError("varn para is different from fun or proc type");
                            }
                        }
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
    else  if(cp<lastp)ParserError("参数个数太少");
//   cout<<"btab[tab[i].reff].psize-1 "<<btab[tab[i].reff].psize-1<<endl;
    emit1(ir_CAL,btab[tab[i].reff].psize-1);    //用户过程或函数调用
    if(tab[i].lev<level)        //符号表所在层次小于当前层次
    {
        emit2(ir_DIS,tab[i].lev,level);  //更新display,将数据区基地址存入display表中
    }
    //  cout<<"This is a actual parameter."<<endl;
}

/*
    void FormalParaSeg();
    函数说明：
    <形式参数段>::=[var]<标识符>{，<标识符>}：<基本类型>
*/
void FormalParaSeg()
{
    int tp,rf,sz,t0;
    bool valpar;    //是否为变量形参
    tp=notyp;       //类型
    rf=0;           //索引
    sz=0;           //数组大小
    if(sym==VAR)
    {
        getSym();
        valpar=false;
    }
    else valpar=true;
    t0=t;
    if(sym==IDEN)
    {
        enter(id,variable);
        getSym();
        while(sym==COMMA)
        {
            getSym();
            if(sym==IDEN)
            {
                enter(id,variable);
                getSym();
            }
            else ParserError("after comma not have iden");
        }
        if(sym==COLON)
        {
            getSym();
            if(sym==INT||sym==CHAR||sym==REAL)
            {
                if(sym==INT)   tp=intt;
                else if(sym==CHAR) tp=charr;
                else tp=real;
                enterVar(t0,t,tp,valpar);    //添加多参数
                getSym();
            }
            else ParserError("para error");
        }
        else ParserError("no :");
    }
    else ParserError("undefine error");
}

/*
    void FormalParaTable();
    函数功能:
    <形式参数表>::='('<形式参数段>{；<形式参数段>}')'

*/
void FormalParaTable()
{
    if(sym==LPARENT)
    {
        getSym();
        FormalParaSeg();
        while(sym==SEMI)
        {
            getSym();
            FormalParaSeg();
        }
        if(sym==RPARENT)
        {
            getSym();
        }
        else ParserError("no ')'");
    }
    else ParserError("no '('");
}

/*
    void proce();
    函数功能：
    <过程首部>::=procedure<标识符>[<形式参数表>];
    <函数首部>::=function<标识符>[<形式参数表>]:<基本类型>
*/
void proce()
{
    bool isfun;
    isfun=(sym==FUNSY);
    getSym();
    if(sym!=IDEN)ParserError("no iden");
    if(isfun) enter(id,functionn);
    else enter(id,procedure);
    tab[t].normal=true;
    getSym();
    SubBlock(isfun);
    if(sym==SEMI) getSym();
    else ParserError("no ;");
    emit(ir_EXP+isfun);
}
