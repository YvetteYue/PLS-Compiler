#include "header.h"     //TODO
#include "def.h"
#include<iostream>
using namespace std;
/*
    void ActualPara(int i);
    ��������:
    <ʵ�ڲ�����>::='('<ʵ�ڲ�����>{,<ʵ�ڲ�����>}')'
*/
void ActualPara(int i)
{
    int lastp,cp,k;
    Item x;
    emit1(ir_MKS,i);        //���ջ
    lastp=btab[tab[i].reff].lastpara;//�ֳ�����в��������һ����������
    cp=i;//���ű��к���λ��
    if(sym==LPARENT) //��
    {
        do
        {
            getSym();
            if(cp>=lastp) ParserError("���������������");//����������ڷ��ű�λ�ñ�һ���β�����λ�ô���ô����
            else
            {
                cp++;
                if(tab[cp].normal) //ֵ�β�
                {
                    x=expr();   //ȡ��������
                    if(x.typ==tab[cp].typ)  //������ʽ���ͺ��β�������ͬ
                    {
                        if(x.reff!=tab[cp].reff)
                        {
                            ParserError("�β���ʵ�����Ͳ�ͬ");
                        }
                    }
                    else if(x.typ==intt&&tab[cp].typ==real)
                    {
                        emit1(ir_FLT,0);    //��ǰֵת���ɸ�����
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
                else   //�����β�
                {
                    if(sym!=IDEN)ParserError("no para");
                    else
                    {
                        k=loc(id);      //���ұ�ʶ������λ��
                        getSym();//
                        if(k!=0)
                        {
                            if(tab[k].obj!=variable) ParserError("not var");
                            x.typ=tab[k].typ;       //����
                            x.reff=tab[k].reff;      //ָ��atab��btab
                            if(tab[k].normal)        //�Ƿ�Ϊֵ�β�
                            {
                                emit2(ir_LDA,tab[k].lev,tab[k].adr);     //ȡʵ�ʲ����ĵ�ַ
                            }
                            else emit2(ir_LOD,tab[k].lev,tab[k].adr);    //ȡʵ�ʲ�����ֵ
                            if(sym==LSQUARE)    //�������Ϊ����
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
    else  if(cp<lastp)ParserError("��������̫��");
//   cout<<"btab[tab[i].reff].psize-1 "<<btab[tab[i].reff].psize-1<<endl;
    emit1(ir_CAL,btab[tab[i].reff].psize-1);    //�û����̻�������
    if(tab[i].lev<level)        //���ű����ڲ��С�ڵ�ǰ���
    {
        emit2(ir_DIS,tab[i].lev,level);  //����display,������������ַ����display����
    }
    //  cout<<"This is a actual parameter."<<endl;
}

/*
    void FormalParaSeg();
    ����˵����
    <��ʽ������>::=[var]<��ʶ��>{��<��ʶ��>}��<��������>
*/
void FormalParaSeg()
{
    int tp,rf,sz,t0;
    bool valpar;    //�Ƿ�Ϊ�����β�
    tp=notyp;       //����
    rf=0;           //����
    sz=0;           //�����С
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
                enterVar(t0,t,tp,valpar);    //��Ӷ����
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
    ��������:
    <��ʽ������>::='('<��ʽ������>{��<��ʽ������>}')'

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
    �������ܣ�
    <�����ײ�>::=procedure<��ʶ��>[<��ʽ������>];
    <�����ײ�>::=function<��ʶ��>[<��ʽ������>]:<��������>
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
