#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
int sym;
int main()
{
    readfile();//�������ļ�
    getSym();//Ԥ��һ������
    init();//��ʼ��
    errormsg();
    enter(" ",variable,notyp,0);/*��ӿշ��ŵ����ű�*/
    enter("read",procedure,notyp,1);/*��ӱ�׼����read,write�����ű�*/
    enter("write",procedure,notyp,2);
    Block();/*ģ�麯��*/
    close();/*�ر��ļ�*/
    //test();/*���ű���ʾ*/
    if(errorFlag) return 0;
    printtable();/*P-codeչʾ*/
    interprett();/*��ӡ�������н��*/
    return 0;
}


