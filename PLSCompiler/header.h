# include <cstdio>
# include <cstring>
# include <algorithm>
# include <string>
# include <cstring>
# include <iomanip>
# include <fstream>
# include <iostream>
# include <stdio.h>
# include <locale.h>
using namespace std;
#define  nkw 27
#define alng 10
#define llng 121
#define emax 322
#define emin -292
#define kmax 15
#define tmax 100
#define bmax 20
#define amax 30
#define c2max 20
#define csmax 30
#define cmax 800
#define lmax 7
#define smax 600
#define ermax 59
#define omax 63
#define xmax 32767
#define nmax 32767
#define lineleng 132
#define linelimit 200
#define stacksize 1450
#define id token
struct Tab
{
    string name;
    int link=0;
    int obj;
    int typ;
    int reff=0;
    bool normal;
    int lev;
    int adr;
};
struct ATab
{
    int typ;
    int sz;
};
struct BTab
{
    int last,lastpara,psize,vsize;
};
struct Code
{
    int f;
    int x,y;
};
struct Conrec //tp表示常量类型：整型，实型，字符，数组，无
{
    int tp,i;//i表示字符和整型的值，r表示实型
    double r;
};
struct Item
{
    int typ,reff=0;
};
enum sy
{
    CONST=0,ARRAY,OF,INT,CHAR,REAL,PROCSY,FUNSY,VAR,
    IF,THEN,ELSE,WHILE,DO,FOR,DOWNTO,TO,BEGIN,END,READ,WRITE,IDEN,
    LBRACE,RBRACE,COMMA,SEMI,PLUS,MINUS,COLON,ASSIGN,LPARENT,RPARENT,MULTI,
    DIV,LSS,LEQ,NEQ,GTR,GEQ,EQL,LSQUARE,RSQUARE,INTCON,REALCON,STRINGCON,CHARCON,POT
};
enum Obj {constant=1,variable,procedure,functionn};
enum Typ {intt=1,real,charr,arrayy,notyp};
enum Instruction {ir_LDA,ir_LOD,ir_LDI,ir_DIS,ir_JMP,ir_JPC,ir_F1U,ir_F2U,ir_F1D,
                  ir_F2D,ir_MKS,ir_CAL,ir_IDX,ir_LDC,ir_LDR,ir_FLT,ir_RED,ir_WRS,ir_WRW,
                  ir_HLT,ir_EXP,ir_EXF,ir_LDT,ir_MUS,ir_STO,ir_EQR,ir_NER,ir_LSR,ir_LER,
                  ir_GTR,ir_GER,ir_EQL,ir_NEQ,ir_LSS,ir_LEQ,ir_GRT,ir_GEQ,ir_ADD,ir_SUB,ir_ADR,ir_SUR,
                  ir_MUL,ir_DIV,ir_MUR,ir_DIR,ir_WRL
                 };
//gen
void error();
void init();
void enter(string name,int obj,int typ,int adr);
void enterarray(int tp,int l,int h);
void enterblock();
void enterreal(double x);
void enter(string name,int k);
void enterVar(int t0,int t1,int tp,bool nrm);
void enterArray(int t0,int t1,int tp,int sz);
void emit(int fct);
void emit1(int fct,int b);
void emit2(int fct,int a,int b);
int loc(string id);
//lex
void readfile();
void getSym();
void close();
//parse
void statement();
Conrec Const();
void ConstDefine();
void ConstDescripSec();
void type();
void VarDesc();
void VarDescripSec();
void FormalParaSeg();
void FormalParaTable();
void proce();
void CompondStm();
void ConditionStm();
void WhileStm();
void ForStm();
void WriteStm();
void ReadStm();
void ActualPara(int i);
Item expr();
Item term();
Item factor();
Item CallArray(Item x);
int resultType(int a,int b);
void assignment(int lv,int ad,int i);
void Block();
void SubBlock(bool isfun);
void ParserError(string str);
void test();
void testbtab(int i);
void testdisplay(int i);
void testtab(int i);
void printtable();
void interprett();
void endskip();
void Error(string str);
void errormsg();


