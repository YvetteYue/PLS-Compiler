#include "header.h"
#include<iostream>
using namespace std;
char token[1000];
Tab tab[tmax];
ATab atab[amax];
BTab btab[bmax];
char stab[smax][smax];
double rconst[cmax];
Code code[cmax];
int inProgram;
int lineNum;
int t;//tab
int a;//atab
int b;//btab
int level;//display
int c2;//rconst
int sx;//stab
int lc;//code
int c1;
int dx;
int display[100];
int errorFlag;
char symbol[][100]= {"CONSTSY","ARRAYSY","OFSY","INTSY","CHARSY","REALSY","PROCSY","FUNSY","VARSY",
                     "IFSY","THENSY","ELSE","WHILE","DOSY","FORSY","DOWNTOSY","TOSY","BEGINSY","ENDSY","READSY","WRITESY","IDENSY",
                     "LBRACESY","RBRACESY","COMMASY","SEMISY","PLUSSY","MINUSSY","COLONSY","ASSIGNSY","LPARENT","RPARENT","MULTI",
                     "DIV","LSS","LEQ","NEQ","GTR","GEQ","EQL","LSQUARE","RSQUARE","INTCON","REALCON","STRINGCON","CHARCON"
                    };
char Objs[][15]= {"null","constant","variable","procedure","function"};
char Typs[][15]= {"null","int","real","char","array","notyp"};
