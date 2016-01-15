#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
Item expr()         //<表达式>::=[+|-]<项>{<加法运算符><项>}
{
    Item x;
    int op;
    if(sym==PLUS||sym==MINUS)   //
    {
        op=sym;
        getSym();
        x=term();
        if(op==MINUS)       //表达式的开始为负号
            emit(ir_MUS);       //求负
    }
    else   x=term();
    while(sym==PLUS||sym==MINUS)
    {
        op=sym;
        getSym();
        Item y=term();
        int type=resultType(x.typ,y.typ);
        x.typ=type;
        // cout<<"x,type"<<type<<endl;
        if(type==real)    //当两个项的符号类型有一个real类型时，返回值为real类型
        {
            x.typ=real;
            if(op==PLUS) emit(ir_ADR);       //ADR
            else emit(ir_SUR);              //SUR
        }
        else if(type==intt||type==charr)
        {
            x.typ=intt;
            if(op==PLUS) emit(ir_ADD);      //ADD
            else emit(ir_SUB);              //SUB
        }
        else ParserError("表达式结果类型错误");
    }
    // cout<<"This is an expression."<<endl;
    return x;
}
Item term()         //<项>::=<因子>{<乘法运算符><因子>}
{
    //cout<<"term也有点问题"<<endl;
    Item x=factor();
    while(sym==DIV||sym==MULTI)     //获取的符号为乘法或者除法时
    {
        int op=sym;
        getSym();
        Item y=factor();
        int type=resultType(x.typ,y.typ);
        x.typ=type;
        if(type==intt||type==charr)       //表达式类型为整型
        {
            x.typ=intt;
            if(op==DIV)      //
            {
                emit(ir_DIV);
            }
            else emit(ir_MUL);
        }
        else if(type==real)       //表达式类型为实型
        {
            if(op==DIV)      //
            {
                emit(ir_DIR);
            }
            else emit(ir_MUR);
        }
        else ParserError("表达式值的类型错误");
    }
    //  cout<<"This is a term."<<endl;
    return x;
}
Item factor()
{
    Item x;
    x.typ=notyp;//类型
    x.reff=0;//指向的符号表类型（atab or btab or null）
    if(sym==INTCON||sym==REALCON||sym==CHARCON) //因子为int,real,char类型三种 实际的数
    {
        if(sym==REALCON)    //实数类型
        {
            x.typ=real;
            enterreal(atof(id));
            emit1(ir_LDR,c1);   //取rconst里面的实数
        }
        else if(sym==CHARCON)   //字符类型
        {
            x.typ=charr;
            emit1(ir_LDC,id[1]);//取字符
        }
        else if(sym==INTCON)    //整数类型
        {
            x.typ=intt;
            emit1(ir_LDC,atoi(id)); //取整数
        }
        x.reff=0;
        getSym();
    }
    else if(sym==LPARENT)
    {
        getSym();
        x=expr();
        if(sym==RPARENT)
        {
            getSym();
        }
        else ParserError("缺右括号");
    }
    else if(sym==IDEN)
    {
        int f;
        int i=loc(id);  //查找标识符在符号表中的位置
        getSym();
        switch(tab[i].obj)
        {
        case constant:  //标识符常量
        {
            x.typ=tab[i].typ;
            x.reff=0;
            if(x.typ==real)
                emit1(ir_LDR,tab[i].adr);   //装入实数
            else emit1(ir_LDC,tab[i].adr);  //装入常量
            break;
        }
        case variable:      //标识符变量
        {
            x.typ=tab[i].typ;
            x.reff=tab[i].reff;
            if(sym==LSQUARE) //数组引用
            {
                if(tab[i].normal)   //判断是变量形参还是值形参
                    f=ir_LDA;    //LDA装地址
                else f=ir_LOD;   //装值
                emit2(f,tab[i].lev,tab[i].adr); //参数
                x=CallArray(x);
                emit(ir_LDT);
            }
            else
            {
                if(tab[i].normal)
                    f=ir_LOD;//直接装入
                else f=ir_LDI;   //间接装入
                emit2(f,tab[i].lev,tab[i].adr);
            }
            break;
        }
        case functionn: //函数返回
        {
            x.typ=tab[i].typ;
            if(tab[i].lev!=0)     //有参数调用参数列表
            {
                ActualPara(i);
            }//参数可有可无
            break;
        }
        case procedure:
            ParserError("过程类型无返回值，不能作为表达式中的项");
            break;
        }
    }
    else ParserError("错误标识符");
    //cout<<"This is a factor."<<endl;
    return x;
}
int resultType(int a,int b)         //整型和实型的转换
{
    if(a==charr)
    {
        a=intt;
    }
    if(b==charr)
    {
        b=intt;
    }
    if(a==notyp||b==notyp)
    {
        ParserError("类型无");
        return notyp;
    }
    else if(a==intt&&b==intt)
    {
        return intt;
    }
    else
    {
        if(a==intt)
        {
            emit1(ir_FLT,1);
        }
        else if(b==intt)
        {
            emit1(ir_FLT,0);
        }
        return real;
    }
}
