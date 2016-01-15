#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
void Condition()        //�ж�����
{
    Item x=expr();
    if(sym==LSS||sym==LEQ||sym==NEQ||sym==GTR||sym==GEQ||sym==EQL)
    {
        int op=sym;
        getSym();
        Item y=expr();
        int type=resultType(x.typ,y.typ);
        if(type==intt) //��������
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
        else ParserError("���ʽ���ʹ���");
    }
    else ParserError("δ֪�ıȽ������");
}
void ConditionStm()     //�������
{
    int lc1,lc2;
    if(sym==IF)
    {
        getSym();
        Condition();
        lc1=lc; //�������1
        emit(ir_JPC);//JPC����ת��
        if(sym==THEN)
        {
            getSym();
            statement();
            if(sym==ELSE)
            {
                getSym();
                lc2=lc;//�������2
                emit(ir_JMP);//������ת��
                code[lc1].y=lc;//LabA
                statement();
                code[lc2].y=lc;//LabB
            }
            else code[lc1].y=lc; //LabA
        }
        else ParserError("if stm no  then");
    }
    else ParserError("����if���");
    //  cout<<"This is a conditional statement."<<endl;
}
void WhileStm()     //ѭ�����
{
    int lc1,lc2;
    if(sym==WHILE)
    {
        getSym();
        lc1=lc;
        Condition();
        lc2=lc;
        emit(ir_JPC);   //JPC����ת��
        if(sym==DO)
        {
            getSym();
            statement();
            emit1(ir_JMP,lc1);  //������ת�Ƶ�ѭ��ѭ����ʼλ��
            code[lc2].y=lc;
        }
        else ParserError("while���ȱ��do�ؼ��֡���");
    }
    else ParserError("undefine");
    //  cout<<"This is a while-statement."<<endl;
}
void ForStm()       //forѭ�����
{
    int cvt,i,f,lc1,lc2;
    Item x;
    if(sym==FOR)
    {
        getSym();
        if(sym==IDEN)
        {
            i=loc(id);//���ű��в��ұ�ʶ������
            getSym();
            if(i==0)
                cvt=intt;
            else if(tab[i].obj==variable)    //��������
            {
                cvt=tab[i].typ;
                if(!tab[i].normal)ParserError("not value var");
                else emit2(ir_LDA,tab[i].lev,tab[i].adr);    //LDAָ������
            }
            else ParserError("for���ѭ���������ʹ���,���Ǳ���");
            if(sym==ASSIGN)
            {
                getSym();
                x=expr();
                if(x.typ!=cvt)
                    ParserError("ѭ��������ĳ�ֵ���ͺ͸�ѭ���������Ͳ�ͳһ");
                if(sym==DOWNTO||sym==TO)
                {
                    if(sym==DOWNTO)
                        f=ir_F1D;   //F1D
                    else f=ir_F1U;   //F1U
                    getSym();
                    x=expr();
                    if(x.typ!=cvt)
                        ParserError("��ֵ��ѭ��������ͳһ");
                    lc1=lc;     //�������1
                    emit(f);    //����ж�
                    if(sym==DO)
                    {
                        getSym();
                        lc2=lc;     //�������2
                        statement();
                        emit1(f+1,lc2);     //�ص�ѭ����ʼλ�� F2U��F2D ������ж�
                        code[lc1].y=lc;     //�˳�ѭ��
                    }
                    else ParserError("for���û��do�ؼ���");
                }
                else ParserError("û��to��downto�ؼ���");
            }
            else ParserError("ѭ��ͷ��û�и�ֵ���");
        }
        else ParserError("for�����û��ѭ������");
    }
    else ParserError("����for���");
    // cout<<"This is a for-statement."<<endl;
}
void WriteStm()             //д���
{
    int i,f;
    Item x,y;
    if(sym==WRITE)
    {
        getSym();
        if(sym==LPARENT)
        {
            getSym();
            if(sym==STRINGCON)      //�ַ���
            {
                //emit1(24,strlen(id));
                emit1(ir_WRS,sx);//TODO
                sx++;
                getSym();
                if(sym==COMMA)
                {
                    getSym();
                    x=expr();   //���ʽ��ֵ
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
    ��ֵ��䣺����1����ʶ�����ڲ㣬����������Ե�ַ����ʶ������λ��
    <��ֵ���>::=<��ʶ��>:=<���ʽ>|<������ʶ��>:=<���ʽ>|<��ʶ��>'['<���ʽ>']':=<���ʽ>
*/
void assignment(int lv,int ad,int i)
{
    Item x,y;
    int f;
    x.typ=tab[i].typ;           //��ʶ������
    x.reff=tab[i].reff;         //��ʶ��ָ���Ӧ���ű��е�λ��
    if(tab[i].normal)       //�����βΣ�ֵ�β�
        f=ir_LDA;
    else f=ir_LOD;
    emit2(f,lv,ad);
    if(sym==LSQUARE) //����
    {
        x=CallArray(x);
    }
    if(sym==ASSIGN)
    {
        getSym();
    }
    else ParserError("û�и�ֵ����");
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
    else ParserError("�д�");
    // cout<<"��ֵ���"<<endl;
}
