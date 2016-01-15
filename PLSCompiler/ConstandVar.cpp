#include "header.h"     //DONE
#include "def.h"
#include<iostream>
using namespace std;
/*
    Conrec Const();
    ��������
    ����ֵ��Conrec: tp->��������    i->�ַ�������ֵ   r->ʵ��ֵ
    ��������:
    <����>::=[+|-](<�޷�������>|<�޷���ʵ��>)|<�ַ�>
*/
Conrec Const()
{
    Conrec c;
    int x,sign=1;
    if(sym==CHARCON)    //�ַ�
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
        if(sym==INTCON)     //����
        {
            getSym();
            c.tp=intt;
            c.i=sign*atoi(id);
        }
        else if(sym==REALCON)   //ʵ��
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
    ��������
    �������ܣ���������
    <��������>::=<��ʶ��>=<����>
*/
void ConstDefine()
{
    if(sym==IDEN)
    {
        enter(id,constant); //�泣��
        getSym();
        if(sym==EQL)
        {
            getSym();
            Conrec c=Const();     //����ֵ
            tab[t].typ=c.tp;
            tab[t].reff=0;
            if(c.tp==real)
            {
                enterreal(c.r);     //��ʵ��
                tab[t].adr=c1;      //tab�д�����
            }
            else tab[t].adr=c.i;
        }
        else ParserError("��������û��'='");
    }
    else ParserError("");
    // cout<<"This is a constant define."<<endl;
}

/*
    void ConstDescripSec();
    ��������
    �������ܣ�����˵�����֣�
    <����˵������>::=const<��������>{,<��������>};
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
    ����: t0  ��ʶ����tab�е���ʼλ��  ��Ե��Ƕ��ʶ������
          t1  ����λ��
    �������ܣ�
    //<����>::=<��������>|array'['<�޷�������>']' of <��������>
    //<��������>::=integer|char|real
*/
void type(int t0,int t1)
{
    int tp,sz;
    tp=notyp;
    if(sym==CHAR||sym==REAL||sym==INT)          //��������
    {
        if(sym==CHAR)tp=charr;
        else if(sym==REAL) tp=real;
        else if(sym==INT)tp=intt;
        getSym();
        enterVar(t0,t1,tp,true);
    }
    else if(sym==ARRAY)             //��������
    {
        getSym();
        if(sym==LSQUARE)
        {
            getSym();
            if(sym==INTCON)         //�±����ͣ��޷�������
            {
                sz=atoi(id);        //���鳤��
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
                            enterArray(t0,t1,sz,tp);    //��ʶ����ʼtab��λ�ã�����λ�ã����鳤�ȣ���������
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
    ����������
    <����˵��>::=<��ʶ��>{,<��ʶ��>}:<����>
*/
void VarDesc()      //
{
    int t0,t1;
    if(sym==IDEN)
    {
        t0=t;//��ֵ
        enter(id,variable);
        getSym();
        while(sym==COMMA)
        {
            getSym();
            if(sym==IDEN)
            {
                enter(id,variable);     //��¼���ű�
                getSym();
            }
            else ParserError("no iden after , ");
        }
        if(sym==COLON)
        {
            t1=t;//����ֵ
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
    ����˵����
    <����˵������>::=var<����˵��>;{<����˵��>;}

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
