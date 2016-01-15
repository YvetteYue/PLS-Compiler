#include"header.h"
#include "def.h"
#include<iostream>
#include<fstream>
using namespace std;
void printtable()
{
    int i;
    char mne[1000][10]=
    {
        " LDA"," LOD"," LDI",
        " DIS"," JMP"," JPC",
        " F1U"," F2U"," F1D",
        " F2D"," MKS"," CAL",
        " IDX"," LDC"," LDR",
        " FLT"," RED"," WRS",
        " WRW"," HLT"," EXP",
        " EXF"," LDT"," MUS",
        " STO"," EQR"," NER",
        " LSR"," LER"," GTR",
        " GER"," EQL"," NEQ",
        " LSS"," LEQ"," GRT",
        " GEQ"," ADD"," SUB",
        " ADR"," SUR"," MUL",
        " DIV"," MUR"," DIR",
        " WRL"
    };
    ofstream fout("result.txt");
    for(int i=0; i<lc; i++)
    {
        cout<<i<<" ";
        cout<<mne[code[i].f]<<",";
        cout<<code[i].f<<",";
        cout<<code[i].x;
        cout<<","<<code[i].y<<endl;
        fout <<i<<" "<<mne[code[i].f]<<","<<code[i].f<<","<<code[i].x<<","<<code[i].y<<endl;
    }
    fout.close();
}
