#include "header.h"     //TODO
#include "def.h"
#include<iostream>
using namespace std;
void ParserError(string str)
{
    Error(str);
}
/*
    void statement();
    ��������:
    <���>::=<��ֵ���>|<�������>|<��ѭ�����>|
        <���̵������>|<�������>|<�����>|<д���>|<forѭ�����>|<��>
*/
void statement()
{
    int i;
    switch(sym)
    {
    case IF:
        ConditionStm();
        break;
    case WHILE:
        WhileStm();
        break;
    case FOR:
        ForStm();
        break;
    case READ:
        ReadStm();
        break;
    case WRITE:
        WriteStm();
        break;
    case BEGIN:
        CompondStm();
        break;
    case IDEN:          //��俪ʼΪ��ʶ��
    {
        i=loc(id);      //���Ҹñ�ʶ���ڷ��ű��е�λ��
        getSym();
        if(i!=0)
        {
            switch(tab[i].obj)
            {
            case constant:      //�����������������󣬼�������Ϊ������
                ParserError("������������������");
                break;
            case variable:      //��俪ʼΪ������˵������Ϊ��ֵ���
                assignment(tab[i].lev,tab[i].adr,i);    //��ʶ�����ڲ㣬����������Ե�ַ����ʶ������λ��
                break;
            case procedure:     //�����ʶ��Ϊ���̣�˵���ǵ��ù���
                if(tab[i].lev!=0)
                    ActualPara(i);      //ʵ��
                break;
            case functionn:     //�����ʶ��Ϊ������˵���Ǻ�������ֵ��ֵ�����Ǹ�ֵ���
                if(tab[i].reff==display[level])     //����ú�����ָ��ķֳ���͵�ǰ�����ֳ���һ�£�˵������������ȷ
                    assignment(tab[i].lev+1,0,i);     //��ǰ��ε��ڲ�,����������Ե�ַ����ʶ������λ�á�
                else  Error("������䲻�Ϸ����Ǹú����ķ������");
                break;
            }
        }
        else ParserError("δ֪��ʶ��");
        break;              //���Ϊ�վͽ���
    }
    }
    // cout<<"This is a statements.\n";
}
void CompondStm()               //������䣺<�������>:=begin<���>{;<���>}end
{
    if(sym==BEGIN)
    {
        getSym();
        statement();
        while(sym==SEMI)
        {
            getSym();
            statement();
        }
        if(sym==END)
        {
            getSym();
        }
        else ParserError("δ֪��ʶ��(ȱ�ֺ�)");
    }
    else ParserError("û��begin��ʼ��");
    // cout<<"This is a compond statement\n";
}
void Block()    //ģ�麯��  <����>::=<�ֳ���>::=[<����˵������>][<����˵������>]{[<����˵������>]|[<����˵������>]}<�������>
{
    int prt,prb,x;
    dx=5;
    prt=t;                  //ȡ��ǰ���ű�λ��
    enter("  ",procedure);
    enterblock();           //���ú����������������ű���
    prb=b;                  //ȡ��ǰ�ֳ����λ��
    display[0]=b;        //��ǰlevel�ķֳ�������Ϊb
    tab[prt].typ=notyp;      //���ú���������Ϊnotyp
    tab[prt].reff=prb;          //���ű��reff����ָ��btab�ֳ�������λ�á�
    btab[prb].lastpara=t;        //���øú�������̵����һ�������ڷ��ű��е�λ��
    btab[prb].psize=dx;         //������Ϣ��+��������
    if(sym==CONST)
    {
        ConstDescripSec();
    }
    if(sym==VAR)
    {
        VarDescripSec();
    }
    btab[prb].vsize=dx;         //������Ϣ��+��������+����
    while(sym==PROCSY||sym==FUNSY)
    {
        proce();
    }
    if(sym==BEGIN)
    {
        inProgram=prb;      //������ڵķ��ű�λ��
        tab[prb].adr=lc;
        CompondStm();
    }
    else ParserError("no 'begin'");
    if(sym==POT)
    {
        emit(ir_HLT);                   //���������־
    }
    else ParserError("ȱ�ٳ��������'.'");

    //  cout<<"Block"<<endl;
}
void SubBlock(bool isfun)       //�����͹���˵������
{
    int prt,prb,x;
    level++;                //�����ڲ㣬�������������ڲ�
    dx=5;
    prt=t;                  //ȡ��ǰ���ű�λ��
    enterblock();           //���ú����������������ű���
    prb=b;                  //ȡ��ǰ�ֳ����λ��
    display[level]=b;        //��ǰlevel�ķֳ�������Ϊb
    tab[prt].typ=notyp;      //���ú���������Ϊnotyp
    tab[prt].reff=prb;          //���ű��reff����ָ��btab�ֳ�������λ�á�
    if(sym==LPARENT) FormalParaTable();  //���ȡ�õ�ǰΪ������,������ʽ��������
    btab[prb].lastpara=t;        //���øú�������̵����һ�������ڷ��ű��е�λ��
    btab[prb].psize=dx;         //������Ϣ��
    if(isfun)                   //�Ǻ��������
    {
        if(sym==COLON)          //����Ϊ:
        {
            getSym();
            if(sym==INT) tab[prt].typ=intt; //��������ֵ����
            else if(sym==CHAR) tab[prt].typ=charr;
            else if(sym==REAL) tab[prt].typ=real;
            else ParserError("�����������ʹ���");
            getSym();
        }
        else ParserError("��������û�з���ֵ");
    }
    if(sym==SEMI) getSym();
    else ParserError("no ;");
    if(sym==CONST)
    {
        ConstDescripSec();
    }
    if(sym==VAR)
    {
        VarDescripSec();
    }
    btab[prb].vsize=dx;
    while(sym==PROCSY||sym==FUNSY)
    {
        proce();
    }
    tab[prt].adr=lc;        //��������̵ĵ�ַΪĿ��������ڵ�ַ
    if(sym==BEGIN)
    {
        CompondStm();
    }
    else ParserError("û�з������");
    //  cout<<"This is a subblock."<<endl;
    level--;            //�˳��ò�
}
