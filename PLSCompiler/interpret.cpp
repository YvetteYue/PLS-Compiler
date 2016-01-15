#include "header.h"
#include "def.h"
#include<iostream>
#include<fstream>
using namespace std;
int pc,topStackIdx,baseIdx,h1,h2,h3,h4;
int lncnt,ocnt,blkcnt,chrcnt;
int fld[4],ps;
int runDisplay[lmax];
Code ir;
enum PS {run=1,fin,caschk,divchk,inxchk,stkchk,linchk,lngchk,redchk};
struct Stacks
{
    int i;
    double r;
    bool b;
};
Stacks stacks[stacksize];
void teststack()
{
    for(int i=0; i<=topStackIdx; i++)
    {
        cout<<"stacks:"<<i<<" "<<stacks[i].i<<endl;
    }
}

void inter()
{
    switch(ir.f)
    {
    case ir_LDA://LDA
        topStackIdx++;
        if(topStackIdx>stacksize)
            ps=stkchk;
        else stacks[topStackIdx].i=runDisplay[ir.x]+ir.y;     //栈顶=调用初始未知+相对地址  (取地址)
        //  cout<<"LDA\n";
        break;
    case ir_LOD://LOD
        // teststack();
        topStackIdx++;
        if(topStackIdx>stacksize)
            ps=stacksize;
        else stacks[topStackIdx]=stacks[runDisplay[ir.x]+ir.y];   //取值
        // cout<<stacks[topStackIdx].i<<">>"<<runDisplay[ir.x]<<"LOD"<<ir.y<<endl;
        //  cout<<"LOD"<<endl;
        break;
    case ir_LDI://LDI 取间接值
        topStackIdx++;
        if(topStackIdx>stacksize)
            ps=stkchk;
        else stacks[topStackIdx]=stacks[stacks[runDisplay[ir.x]+ir.y].i];
        break;
    case ir_DIS://DIS
        h1=ir.y;
        h2=ir.x;
        h3=baseIdx;
        do
        {
            runDisplay[h1]=h3;
            h1=h1-1;
            h3=stacks[h3+2].i;
        }
        while(h1!=h2);
        break;
    case ir_JMP://JPC
        pc=ir.y;
        break;
    case ir_JPC:
        if(!stacks[topStackIdx].b)
            pc=ir.y;
        topStackIdx--;
        break;
    case ir_F1U:
        h1=stacks[topStackIdx-1].i;
        if(h1<=stacks[topStackIdx].i)
            stacks[stacks[topStackIdx-2].i].i=h1;
        else
        {
            topStackIdx-=3;
            pc=ir.y;
        }
        break;
    case ir_F2U:
        h2=stacks[topStackIdx-2].i;
        h1=stacks[h2].i+1;
        if(h1<=stacks[topStackIdx].i)
        {
            stacks[h2].i=h1;
            pc=ir.y;
        }
        else topStackIdx-=3;
        break;
    case ir_F1D:
        h1=stacks[topStackIdx-1].i;
        if(h1>=stacks[topStackIdx].i)
        {
            stacks[stacks[topStackIdx-2].i].i=h1;
        }
        else
        {
            pc=ir.y;
            topStackIdx-=3;
        }
        break;
    case ir_F2D:
        h2=stacks[topStackIdx-2].i;
        h1=stacks[h2].i-1;
        if(h1>=stacks[topStackIdx].i)
        {
            stacks[h2].i=h1;
            pc=ir.y;
        }
        else topStackIdx-=3;
        break;
    case ir_MKS:    //MKS
        h1=btab[tab[ir.y].reff].vsize;
        if(topStackIdx+h1>stacksize)
            ps=stkchk;
        else
        {
            topStackIdx+=5;
            stacks[topStackIdx-1].i=h1-1;
            stacks[topStackIdx].i=ir.y;
        }
        //  cout<<"MKS"<<endl;
        break;
    case ir_CAL:    //CAL
        h1=topStackIdx-ir.y;    //
        h2=stacks[h1+4].i;      //
        h3=tab[h2].lev;
        runDisplay[h3+1]=h1;
        h4 = stacks[h1 + 3].i + h1;
        stacks[h1 + 1].i = pc;
        stacks[h1 + 2].i = runDisplay[h3];
        stacks[h1 + 3].i = baseIdx;
        for (h3 = topStackIdx + 1; h3<=h4; h3++)
            stacks[h3].i = 0;
        baseIdx = h1;
        topStackIdx = h4;
        pc = tab[h2].adr;
        //   cout<<"dump CALL"<<pc<<"baseIdx"<<baseIdx<<endl;
        break;
    case ir_IDX://IDX 去下标变量地址
        h1=ir.y;
        h2=0;
        h3=stacks[topStackIdx].i;
        //cout<<h2<<"??"<<h3<<"size"<<atab[h1].sz<<endl;
        if(h3<h2) ps=inxchk;
        else if(h3>atab[h1].sz)
        {
            ps=inxchk;
        }
        else
        {
            topStackIdx--;
            stacks[topStackIdx].i=stacks[topStackIdx].i+(h3-h2);
        }
        break;
    case ir_LDC:    //LDC 取常量
        topStackIdx++;
        // cout<<"top"<<topStackIdx<<endl;
        if(topStackIdx>stacksize)
            ps=stkchk;
        else
            stacks[topStackIdx].i=ir.y;
        //   cout<<"LDC"<<endl;
        break;
    case ir_LDR:
        topStackIdx++;
        if(topStackIdx>stacksize)
            ps=stkchk;
        else stacks[topStackIdx].r=rconst[ir.y];
        break;
    case ir_FLT:
        h1=topStackIdx-ir.y;
        stacks[h1].r=stacks[h1].i;
        break;
    case ir_RED://RED
        int a;
        switch(ir.y)
        {
        case 1:
            scanf("%d",&stacks[stacks[topStackIdx].i].i);
            break;
        case 2:
            scanf("%lf",&stacks[stacks[topStackIdx].i].r);
            break;
        case 3:
            char ss[2];
            scanf("%s",&ss);
            stacks[stacks[topStackIdx].i].i=ss[0];
            break;
        }
        topStackIdx--;
        // cout<<"RED\n";
        break;
    case ir_WRS://WRS
        // topStackIdx--;
        if(chrcnt>lineleng)
            ps=lngchk;
        cout<<stab[ir.y];
        // cout<<"WRS"<<endl;
        break;
    case ir_WRW://WRW 写
        chrcnt+=5;
        if(chrcnt>lineleng)
            ps=lngchk;
        else
        {
            // cout<<stacks[topStackIdx].i<<"错误很多啊"<<ir.y<<endl;
            switch(ir.y)
            {
            case 1:
                printf("%d",stacks[topStackIdx].i);   //整型
                break;
            case 2:
                printf("%lf",stacks[topStackIdx].r);    //实数
                break;
            case 3:
                printf("%c",stacks[topStackIdx].i);    //字符
                break;
            default:
                cout<<"错误类型"<<endl;
            }
        }
        topStackIdx--;
        // cout<<"WRW"<<endl;
        break;
    case ir_EXP:
        topStackIdx= baseIdx - 1;
        pc = stacks[baseIdx+1].i;
        baseIdx= stacks[baseIdx+3].i;
        //  cout<<"EXP"<<endl;
        break;
    case ir_EXF: //EXF
        topStackIdx= baseIdx;
        //    cout<<baseIdx<<"baseIdx";
        pc= stacks[baseIdx+1].i;
        baseIdx= stacks[baseIdx+3].i;
        // cout<<"EXF"<<baseIdx<<endl;
        break;
    case ir_LDT://LDT
        stacks[topStackIdx]=stacks[stacks[topStackIdx].i];
        break;
    case ir_MUS://MUS
        stacks[topStackIdx].i=-stacks[topStackIdx].i;
        break;
    case ir_STO://STO
        //  cout<<"STO"<<stacks[topStackIdx].i<<endl;
        stacks[stacks[topStackIdx-1].i]=stacks[topStackIdx];
        topStackIdx-=2;
        break;
    case ir_WRL://WRL
        printf("\n");
        lncnt+=1;
        chrcnt=0;
        if(chrcnt>linelimit)
            ps=linchk;
        //  cout<<"WRL"<<endl;
        break;
    case ir_HLT:
        ps=fin;//HTL
        //cout<<"HTL"<<endl;
        break;
    case ir_EQR:
        topStackIdx--;
        stacks[topStackIdx].b=(stacks[topStackIdx].r==stacks[topStackIdx+1].r);
        // cout<<stacks[topStackIdx].b<<"  "<<stacks[topStackIdx].r<<"  "<<stacks[topStackIdx+1].r<<endl;
        break;
    case ir_NER:
        topStackIdx= topStackIdx - 1;
        stacks[topStackIdx].b= stacks[topStackIdx].r != stacks[topStackIdx+1].r;
        break;
    case ir_LSR:
        topStackIdx=topStackIdx-1;
        stacks[topStackIdx].b = stacks[topStackIdx].r < stacks[topStackIdx+1].r;
        break;
    case  ir_LER:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b = stacks[topStackIdx].r <= stacks[topStackIdx+1].r;
        break;
    case ir_GTR:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b = stacks[topStackIdx].r > stacks[topStackIdx+1].r;
        break;
    case ir_GER:
        topStackIdx=topStackIdx-1;
        stacks[topStackIdx].b= stacks[topStackIdx].r >= stacks[topStackIdx+1].r;
        break;
    case ir_EQL:
        topStackIdx=topStackIdx-1;
        stacks[topStackIdx].b= stacks[topStackIdx].i ==stacks[topStackIdx+1].i;
        break;
    case ir_NEQ:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b= stacks[topStackIdx].i != stacks[topStackIdx+1].i;
        break;
    case ir_LSS:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b= stacks[topStackIdx].i < stacks[topStackIdx+1].i;
        break;
    case ir_LEQ:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b= stacks[topStackIdx].i <= stacks[topStackIdx+1].i;
        break;
    case ir_GRT:
        topStackIdx=topStackIdx - 1;
        stacks[topStackIdx].b= stacks[topStackIdx].i > stacks[topStackIdx+1].i;
        break;
    case ir_GEQ:
        topStackIdx--;
        stacks[topStackIdx].b=stacks[topStackIdx].i>=stacks[topStackIdx+1].i;
        break;
    case  ir_ADD:
        topStackIdx--;
        stacks[topStackIdx].i= stacks[topStackIdx].i + stacks[topStackIdx+1].i;
        break;
    case ir_SUB:
        topStackIdx--;
        stacks[topStackIdx].i = stacks[topStackIdx].i - stacks[topStackIdx+1].i;
        break;
    case  ir_ADR:
        topStackIdx--;
        stacks[topStackIdx].r = stacks[topStackIdx].r + stacks[topStackIdx+1].r;
        break;
    case   ir_SUR:
        topStackIdx--;
        stacks[topStackIdx].r = stacks[topStackIdx].r - stacks[topStackIdx+1].r;
        break;
    case   ir_MUL:
        topStackIdx--;
        stacks[topStackIdx].i = stacks[topStackIdx].i * stacks[topStackIdx+1].i;
        break;
    case   ir_DIV:
        topStackIdx--;
        if(stacks[topStackIdx+1].i == 0)
            ps = divchk;
        else stacks[topStackIdx].i = stacks[topStackIdx].i / stacks[topStackIdx+1].i;
        // cout<<"???"<<ps<<divchk<<endl;
        break;
    case ir_MUR:
        topStackIdx--;
        stacks[topStackIdx].r=stacks[topStackIdx].r*stacks[topStackIdx+1].r;
        // cout<<stacks[topStackIdx].r<<"。。。。"<<stacks[topStackIdx+1].r<<endl;
        break;
    case ir_DIR:
        topStackIdx--;
        if(stacks[topStackIdx+1].r == 0)
            ps = divchk;
        else
            stacks[topStackIdx].r=stacks[topStackIdx].r/stacks[topStackIdx+1].r;
        break;
    default:
        cout<<ir.f<<"未定义"<<endl;
    }
}
void interprett()       //解释执行程序
{
    //cout<<inProgram<<endl;
    stacks[1].i=0;
    stacks[2].i=0;
    stacks[3].i=-1;
    stacks[4].i=inProgram;   //程序入口
    runDisplay[0]=0;
    runDisplay[1]=0;
    topStackIdx=btab[2].vsize-1;       //topStackIdx表示全局变量区最后一个但与，即
    baseIdx=0;           //基地址
    pc=tab[stacks[4].i].adr;     //程序入口地址
    // cout<<pc<<endl;
    lncnt=0;
    ocnt=0;
    chrcnt=0;
    ps=run;
    do
    {
        ir=code[pc];
        pc++;
        ocnt++;
        inter();
    }
    while(ps==run);
    if(ps==divchk) cout<<"error:division by 0";
    else if(ps==inxchk) cout<<"error:invalid index";
    else if(ps==redchk)cout<<"error:reading pass end or file";
}
