#include "header.h"     //DONE
#include "def.h"
#include<iostream>
using namespace std;
void init()
{
    lc=0;//code索引
    t=-1;//tab索引
    a=0;//atab
    b=1;//btab
    sx=0;//stab
    c2=0;//rconst
    display[0]=1;//diaplay
    level=1;
}
void enter(string name,int obj,int typ,int adr)         //存入符号表(标准函数)
{
    t++;
    tab[t].name=name;   //姓名
    tab[t].link=t-1;    //上一个标识符在tab中的位置
    tab[t].obj=obj;     //标识符种类（常量,变量,过程,函数）
    tab[t].typ=typ;     //标识符类型或返回值类型
    tab[t].reff=0;      //指向的函数过程位置或数组类型名位置
    tab[t].normal=true; //是值形参还是变量形参
    tab[t].lev=0;       //当前所在层
    tab[t].adr=adr;     //函数所在位置
}
void error()
{
    cout<<"error!"<<endl;
    exit(1);
}

void enterblock()   //分程序模块
{
    if(b==bmax) error();    //函数过多超过限制报错
    else
    {
        b++;                //分程序索引加一
        btab[b].last=0;     //初始化 当前分程序中最后一个标识符在tab中的位置
        btab[b].lastpara=0;  //函数或过程中最后一个参数所在位置
    }
    // cout<<"erterblock"<<endl;
}
void enterreal(double x)    //存取实数 没懂！！！
{
    //cout<<"enterreal没懂"<<endl;
    if(c2==c2max-1) error();
    else
    {
        rconst[c2+1]=x;
        c1=1;
        while(rconst[c1]!=x)
        {
            c1++;
        }
        if(c1>c2) c2=c1;
    }
    //  cout<<"enterreal"<<endl;
}
void emit(int fct)              //一个操作数
{
    if(lc==cmax) error();
    code[lc].f=fct;
    lc++;
    //  cout<<"emit"<<endl;
}
void emit1(int fct,int b)         //两个
{
    if(lc==cmax) error();
    code[lc].f=fct;
    code[lc].y=b;
    lc++;
    //cout<<"emit1"<<endl;
}
void emit2(int fct,int a,int b)         //三个操作数
{
    if(lc==cmax) error();
    code[lc].f=fct;
    code[lc].x=a;
    code[lc].y=b;
    lc++;
    //  cout<<"emit2"<<endl;
}
void enter(string id,int k)   //参数1：符号长度;参数2：标识符种类，初始化添加
{
    int j,l;
    if(t==tmax) Error("enter array out ");
    else
    {
        tab[0].name=id;         //登录符号表
        j=btab[display[level]].last;    //j表示该层最后一个变量的位置
        l=j;                            //参数所在位置
        while(tab[j].name!=id)      //从近到远查找字符号位置
        {
            j=tab[j].link;
        }
        if(j!=0) Error("defined");  //定义过了
        else
        {
            t++;        //符号表位置增加
            tab[t].name=id;     //姓名
            tab[t].link=l;      //索引指向上一个标识符
            tab[t].obj=k;       //标识符类型常量，变量，过程，函数
            tab[t].typ=notyp;   //int，real,bool,char,array
            tab[t].reff=0;      //atab or btab
            tab[t].lev=level;   //当前所在层
            tab[t].adr=0;
            tab[t].normal=false;//初始化
            btab[display[level]].last=t;    //修改分程序表中该层的最后一个标识符的位置
        }
    }
//   cout<<"enter 范围内"<<endl;
}
void enterVar(int t0,int t1,int tp,bool nrm)     //在tab中为变量添加类型，多变量
{
    while(t0<t1)                    //t0 符号表位置,t1,typ表示变量类型
    {
        t0++;
        tab[t0].reff=0;
        tab[t0].typ=tp;             //类型
        tab[t0].lev=level;          //当前层
        tab[t0].adr=dx;             //索引
        tab[t0].normal=nrm;        //值形参，变量名 true 变量形参 false
        dx=dx+1;
    }
}
void enterArray(int t0,int t1,int sz,int tp)    //创建数组类型,在tab中为数组类型变量添加类型
{
    if(abs(sz)>xmax)                            //多数组
    {
        Error("table out");
        sz=0;
    }
    if(a==amax)Error("table out ");
    else
    {
        a++;                //索引加一
        atab[a].typ=tp;     //类型
        atab[a].sz=sz;      //数组长度
    }
    while(t0<t1)
    {
        t0++;
        tab[t0].typ=arrayy;     //数组类型
        tab[t0].reff=a;         //指向atab中的位置
        tab[t0].lev=level;      //层次
        tab[t0].adr=dx;         //参数存储相对位置
        tab[t0].normal=true;    //变量
        dx=dx+sz;               //栈的信息区长度
    }
//    cout<<"erterarray"<<endl;
}
int loc(string id)              //查找索引的位置
{
    int i,j;
    i=level;                    //当前层次
    tab[0].name=id;
    do
    {
        j=btab[display[i]].last;    //当前符号表最后一个名字
        while(tab[j].name!=id)      //查找字母位置
        {
            j=tab[j].link;          //往上查找
        }
        i--;
    }
    while(i>=0&&j==0);              //level大于等于0并且link等于0，即没查找到也没有到达最开始位置
    if(j==0)ParserError("未定义该标识符");
    return j;
}
Item CallArray(Item x) //数组引用！【<表达式>】
{
    int a,j;
    if(sym!=LSQUARE)    //【
        Error("no [");
    else
    {
        getSym();
        Item y=expr();      //数组下标为表达式
        if(x.typ!=arrayy)   //如果传入的参数x不是数组类型
        {
            Error("not array type");
        }
        else
        {
            a=x.reff;       //取数组类型指向的atab中的位置
            if(y.typ!=intt)     //如果数组中括号内的参数不是int那么报错！！要处理实数的情况继续运行
            {
                Error("index not int type");
            }
            else            //否则运行结果，将数组类型和索引返回。
            {
                //  cout<<Typs[x.typ]<<endl;
                emit1(ir_IDX,a);    //取下标变量地址
                x.typ=atab[a].typ;  //数组类型
                x.reff=0;           //无索引
            }
        }
    }
    if(sym==RSQUARE)    //】
        getSym();
    else ParserError("no ]");
    return x;
}
