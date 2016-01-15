#include "header.h"     //DONE
#include "def.h"
#include<iostream>
using namespace std;
void init()
{
    lc=0;//code����
    t=-1;//tab����
    a=0;//atab
    b=1;//btab
    sx=0;//stab
    c2=0;//rconst
    display[0]=1;//diaplay
    level=1;
}
void enter(string name,int obj,int typ,int adr)         //������ű�(��׼����)
{
    t++;
    tab[t].name=name;   //����
    tab[t].link=t-1;    //��һ����ʶ����tab�е�λ��
    tab[t].obj=obj;     //��ʶ�����ࣨ����,����,����,������
    tab[t].typ=typ;     //��ʶ�����ͻ򷵻�ֵ����
    tab[t].reff=0;      //ָ��ĺ�������λ�û�����������λ��
    tab[t].normal=true; //��ֵ�βλ��Ǳ����β�
    tab[t].lev=0;       //��ǰ���ڲ�
    tab[t].adr=adr;     //��������λ��
}
void error()
{
    cout<<"error!"<<endl;
    exit(1);
}

void enterblock()   //�ֳ���ģ��
{
    if(b==bmax) error();    //�������೬�����Ʊ���
    else
    {
        b++;                //�ֳ���������һ
        btab[b].last=0;     //��ʼ�� ��ǰ�ֳ��������һ����ʶ����tab�е�λ��
        btab[b].lastpara=0;  //��������������һ����������λ��
    }
    // cout<<"erterblock"<<endl;
}
void enterreal(double x)    //��ȡʵ�� û��������
{
    //cout<<"enterrealû��"<<endl;
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
void emit(int fct)              //һ��������
{
    if(lc==cmax) error();
    code[lc].f=fct;
    lc++;
    //  cout<<"emit"<<endl;
}
void emit1(int fct,int b)         //����
{
    if(lc==cmax) error();
    code[lc].f=fct;
    code[lc].y=b;
    lc++;
    //cout<<"emit1"<<endl;
}
void emit2(int fct,int a,int b)         //����������
{
    if(lc==cmax) error();
    code[lc].f=fct;
    code[lc].x=a;
    code[lc].y=b;
    lc++;
    //  cout<<"emit2"<<endl;
}
void enter(string id,int k)   //����1�����ų���;����2����ʶ�����࣬��ʼ�����
{
    int j,l;
    if(t==tmax) Error("enter array out ");
    else
    {
        tab[0].name=id;         //��¼���ű�
        j=btab[display[level]].last;    //j��ʾ�ò����һ��������λ��
        l=j;                            //��������λ��
        while(tab[j].name!=id)      //�ӽ���Զ�����ַ���λ��
        {
            j=tab[j].link;
        }
        if(j!=0) Error("defined");  //�������
        else
        {
            t++;        //���ű�λ������
            tab[t].name=id;     //����
            tab[t].link=l;      //����ָ����һ����ʶ��
            tab[t].obj=k;       //��ʶ�����ͳ��������������̣�����
            tab[t].typ=notyp;   //int��real,bool,char,array
            tab[t].reff=0;      //atab or btab
            tab[t].lev=level;   //��ǰ���ڲ�
            tab[t].adr=0;
            tab[t].normal=false;//��ʼ��
            btab[display[level]].last=t;    //�޸ķֳ�����иò�����һ����ʶ����λ��
        }
    }
//   cout<<"enter ��Χ��"<<endl;
}
void enterVar(int t0,int t1,int tp,bool nrm)     //��tab��Ϊ����������ͣ������
{
    while(t0<t1)                    //t0 ���ű�λ��,t1,typ��ʾ��������
    {
        t0++;
        tab[t0].reff=0;
        tab[t0].typ=tp;             //����
        tab[t0].lev=level;          //��ǰ��
        tab[t0].adr=dx;             //����
        tab[t0].normal=nrm;        //ֵ�βΣ������� true �����β� false
        dx=dx+1;
    }
}
void enterArray(int t0,int t1,int sz,int tp)    //������������,��tab��Ϊ�������ͱ����������
{
    if(abs(sz)>xmax)                            //������
    {
        Error("table out");
        sz=0;
    }
    if(a==amax)Error("table out ");
    else
    {
        a++;                //������һ
        atab[a].typ=tp;     //����
        atab[a].sz=sz;      //���鳤��
    }
    while(t0<t1)
    {
        t0++;
        tab[t0].typ=arrayy;     //��������
        tab[t0].reff=a;         //ָ��atab�е�λ��
        tab[t0].lev=level;      //���
        tab[t0].adr=dx;         //�����洢���λ��
        tab[t0].normal=true;    //����
        dx=dx+sz;               //ջ����Ϣ������
    }
//    cout<<"erterarray"<<endl;
}
int loc(string id)              //����������λ��
{
    int i,j;
    i=level;                    //��ǰ���
    tab[0].name=id;
    do
    {
        j=btab[display[i]].last;    //��ǰ���ű����һ������
        while(tab[j].name!=id)      //������ĸλ��
        {
            j=tab[j].link;          //���ϲ���
        }
        i--;
    }
    while(i>=0&&j==0);              //level���ڵ���0����link����0����û���ҵ�Ҳû�е����ʼλ��
    if(j==0)ParserError("δ����ñ�ʶ��");
    return j;
}
Item CallArray(Item x) //�������ã���<���ʽ>��
{
    int a,j;
    if(sym!=LSQUARE)    //��
        Error("no [");
    else
    {
        getSym();
        Item y=expr();      //�����±�Ϊ���ʽ
        if(x.typ!=arrayy)   //�������Ĳ���x������������
        {
            Error("not array type");
        }
        else
        {
            a=x.reff;       //ȡ��������ָ���atab�е�λ��
            if(y.typ!=intt)     //��������������ڵĲ�������int��ô������Ҫ����ʵ���������������
            {
                Error("index not int type");
            }
            else            //�������н�������������ͺ��������ء�
            {
                //  cout<<Typs[x.typ]<<endl;
                emit1(ir_IDX,a);    //ȡ�±������ַ
                x.typ=atab[a].typ;  //��������
                x.reff=0;           //������
            }
        }
    }
    if(sym==RSQUARE)    //��
        getSym();
    else ParserError("no ]");
    return x;
}
