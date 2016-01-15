#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
Item expr()         //<���ʽ>::=[+|-]<��>{<�ӷ������><��>}
{
    Item x;
    int op;
    if(sym==PLUS||sym==MINUS)   //
    {
        op=sym;
        getSym();
        x=term();
        if(op==MINUS)       //���ʽ�Ŀ�ʼΪ����
            emit(ir_MUS);       //��
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
        if(type==real)    //��������ķ���������һ��real����ʱ������ֵΪreal����
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
        else ParserError("���ʽ������ʹ���");
    }
    // cout<<"This is an expression."<<endl;
    return x;
}
Item term()         //<��>::=<����>{<�˷������><����>}
{
    //cout<<"termҲ�е�����"<<endl;
    Item x=factor();
    while(sym==DIV||sym==MULTI)     //��ȡ�ķ���Ϊ�˷����߳���ʱ
    {
        int op=sym;
        getSym();
        Item y=factor();
        int type=resultType(x.typ,y.typ);
        x.typ=type;
        if(type==intt||type==charr)       //���ʽ����Ϊ����
        {
            x.typ=intt;
            if(op==DIV)      //
            {
                emit(ir_DIV);
            }
            else emit(ir_MUL);
        }
        else if(type==real)       //���ʽ����Ϊʵ��
        {
            if(op==DIV)      //
            {
                emit(ir_DIR);
            }
            else emit(ir_MUR);
        }
        else ParserError("���ʽֵ�����ʹ���");
    }
    //  cout<<"This is a term."<<endl;
    return x;
}
Item factor()
{
    Item x;
    x.typ=notyp;//����
    x.reff=0;//ָ��ķ��ű����ͣ�atab or btab or null��
    if(sym==INTCON||sym==REALCON||sym==CHARCON) //����Ϊint,real,char�������� ʵ�ʵ���
    {
        if(sym==REALCON)    //ʵ������
        {
            x.typ=real;
            enterreal(atof(id));
            emit1(ir_LDR,c1);   //ȡrconst�����ʵ��
        }
        else if(sym==CHARCON)   //�ַ�����
        {
            x.typ=charr;
            emit1(ir_LDC,id[1]);//ȡ�ַ�
        }
        else if(sym==INTCON)    //��������
        {
            x.typ=intt;
            emit1(ir_LDC,atoi(id)); //ȡ����
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
        else ParserError("ȱ������");
    }
    else if(sym==IDEN)
    {
        int f;
        int i=loc(id);  //���ұ�ʶ���ڷ��ű��е�λ��
        getSym();
        switch(tab[i].obj)
        {
        case constant:  //��ʶ������
        {
            x.typ=tab[i].typ;
            x.reff=0;
            if(x.typ==real)
                emit1(ir_LDR,tab[i].adr);   //װ��ʵ��
            else emit1(ir_LDC,tab[i].adr);  //װ�볣��
            break;
        }
        case variable:      //��ʶ������
        {
            x.typ=tab[i].typ;
            x.reff=tab[i].reff;
            if(sym==LSQUARE) //��������
            {
                if(tab[i].normal)   //�ж��Ǳ����βλ���ֵ�β�
                    f=ir_LDA;    //LDAװ��ַ
                else f=ir_LOD;   //װֵ
                emit2(f,tab[i].lev,tab[i].adr); //����
                x=CallArray(x);
                emit(ir_LDT);
            }
            else
            {
                if(tab[i].normal)
                    f=ir_LOD;//ֱ��װ��
                else f=ir_LDI;   //���װ��
                emit2(f,tab[i].lev,tab[i].adr);
            }
            break;
        }
        case functionn: //��������
        {
            x.typ=tab[i].typ;
            if(tab[i].lev!=0)     //�в������ò����б�
            {
                ActualPara(i);
            }//�������п���
            break;
        }
        case procedure:
            ParserError("���������޷���ֵ��������Ϊ���ʽ�е���");
            break;
        }
    }
    else ParserError("�����ʶ��");
    //cout<<"This is a factor."<<endl;
    return x;
}
int resultType(int a,int b)         //���ͺ�ʵ�͵�ת��
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
        ParserError("������");
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
