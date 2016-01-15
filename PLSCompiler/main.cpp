#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
int sym;
int main()
{
    readfile();//读代码文件
    getSym();//预读一个符号
    init();//初始化
    errormsg();
    enter(" ",variable,notyp,0);/*添加空符号到符号表*/
    enter("read",procedure,notyp,1);/*添加标准函数read,write到符号表*/
    enter("write",procedure,notyp,2);
    Block();/*模块函数*/
    close();/*关闭文件*/
    //test();/*符号表显示*/
    if(errorFlag) return 0;
    printtable();/*P-code展示*/
    interprett();/*打印程序运行结果*/
    return 0;
}


