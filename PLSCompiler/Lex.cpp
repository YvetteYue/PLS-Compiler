#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
ifstream in;
char buffer[256];
char ch;
#define id token
int num;
int p;//指针位置
int len;
void LexError()
{
    cout<<"LexError:Unknown identifier"<<endl;
    // exit(1);
}
void getch()
{
    ch=buffer[p];
    p++;
}
void retract()
{
    p--;
    ch=buffer[p-1];
}
void getSym()
{
    do
    {
        if(p>=len)
        {
            if(!in.eof())
            {
                in.getline(buffer,200);
                lineNum++;
                p=0;
                len=strlen(buffer);
            }
            else return;
        }
        getch();
        while(ch==' '||ch=='\n'||ch==' '||ch==9)getch();
    }
    while(p>len);
    if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')) //字母开头
    {
        int i=0;
        while((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9'))
        {
            token[i]=ch;
            i++;
            getch();
        }
        retract();
        token[i]='\0';
        if(strcmp(token,"const")==0)
        {
            sym=CONST;
        }
        else if(strcmp(token,"array")==0)
        {
            sym=ARRAY;
        }
        else if(strcmp(token,"of")==0)
        {
            sym=OF;
        }
        else if(strcmp(token,"integer")==0)
        {
            sym=INT;
        }
        else if(strcmp(token,"char")==0)
        {
            sym=CHAR;
        }
        else if(strcmp(token,"real")==0)
        {
            sym=REAL;
        }
        else if(strcmp(token,"procedure")==0)
        {
            sym=PROCSY;
        }
        else if(strcmp(token,"function")==0)
        {
            sym=FUNSY;
        }
        else if(strcmp(token,"var")==0)
        {
            sym=VAR;
        }
        else if(strcmp(token,"if")==0)
        {
            sym=IF;
        }
        else if(strcmp(token,"then")==0)
        {
            sym=THEN;
        }
        else if(strcmp(token,"else")==0)
        {
            sym=ELSE;
        }
        else if(strcmp(token,"while")==0)
        {
            sym=WHILE;
        }
        else if(strcmp(token,"do")==0)
        {
            sym=DO;
        }
        else if(strcmp(token,"for")==0)
        {
            sym=FOR;
        }
        else if(strcmp(token,"downto")==0)
        {
            sym=DOWNTO;
        }
        else if(strcmp(token,"to")==0)
        {
            sym=TO;
        }
        else if(strcmp(token,"begin")==0)
        {
            sym=BEGIN;
        }
        else if(strcmp(token,"end")==0)
        {
            sym=END;
        }
        else if(strcmp(token,"read")==0)
        {
            sym=READ;
        }
        else if(strcmp(token,"write")==0)
        {
            sym=WRITE;
        }
        else  sym=IDEN;
        //  cout<<symbol[sym]<<"<<"<<token<<">>>>"<<endl;
        return;
    }
    else if(ch=='{') sym=LBRACE;
    else if(ch=='}')sym=RBRACE;
    else if(ch==',')sym=COMMA;
    else if(ch==';')sym=SEMI;
    else if(ch=='+')sym=PLUS;
    else if(ch=='-')sym=MINUS;
    else if(ch==':')
    {
        getch();
        if(ch=='=')
        {
            sym=ASSIGN;
            //       cout<<symbol[sym]<<":="<<endl;
            return;
        }
        else
        {
            sym=COLON;
            retract();
        }
    }
    else if(ch=='(')sym=LPARENT;
    else if(ch==')')sym=RPARENT;
    else if(ch=='*')sym=MULTI;
    else if(ch=='/')sym=DIV;
    else if(ch=='<')
    {
        getch();
        if(ch=='=')
        {
            sym=LEQ;
            //    cout<<symbol[sym]<<" <="<<endl;
            return;
        }
        else if(ch=='>')
        {
            sym=NEQ;
            //   cout<<symbol[sym]<<" <>"<<endl;
            return;
        }
        else
        {
            sym=LSS;
            retract();
        }

    }
    else if(ch=='>')
    {
        getch();
        if(ch=='=')
        {
            sym=GEQ;
            //  cout<<sym<< ">="<<endl;
            return;
        }
        else
        {
            sym=GTR;
            retract();
        }

    }
    else if(ch=='=')
    {
        sym=EQL;
    }
    else if(ch=='.')
    {
        //cout<<"pointtt"<<endl;
        sym=POT;
        //cout<<POT<<"..."<<sym<<endl;
    }
    else if(ch=='[')
    {
        sym=LSQUARE;
    }
    else if(ch==']')
    {
        sym=RSQUARE;
    }
    else if(ch>='0'&&ch<='9')
    {
        int j=0;
        while(ch>='0'&&ch<='9')
        {
            token[j]=ch;
            j++;
            getch();
        }
        if(ch=='.')
        {
            getch();
            if(ch>='0'&&ch<='9')
            {
                token[j]='.';
                j++;
            }
            while(ch>='0'&&ch<='9')
            {
                token[j]=ch;
                j++;
                getch();
            }
            sym=REALCON;
        }
        else sym=INTCON;
        token[j]='\0';
        //    cout<<symbol[sym]<<" "<<token<<endl;
        retract();
        return;
    }
    else if(ch=='"')
    {
        int k=0;
        token[k]=ch;
        // k++;
        getch();
        while(ch==32||ch==33||(ch>=35&&ch<=126))
        {
            token[k]=ch;
            k++;
            getch();
        }
        if(ch=='"')
        {
            // token[k]='"';
            token[k]='\0';
            sym=STRINGCON;
            strcpy(stab[sx],token);
        }
        else LexError();
        return;
    }
    else if(ch=='\'')
    {
        int k=0;
        token[k]=ch;
        k++;
        getch();
        if((ch<='Z'&&ch>='A')||(ch<='z'&&ch>='a')||(ch<='9'&&ch>='0'))
        {
            token[k++]=ch;
            getch();
            if(ch=='\'')
            {
                token[k++]=ch;
                token[k]='\0';
                sym=CHARCON;
            }
            //cout<<symbol[sym]<<" "<<token<<endl;
            return;
        }
        else LexError();
    }

    // getch();
    //cout<<symbol[sym]<<" "<<ch<<"<<"<<endl;
    return;
}
void readfile()
{
    char filename[10000];
    // freopen("x.txt","r",stdin);
    printf("请输入程序文件的路径：例如：E:\\a.txt\n");
    cin>>filename;
    in.open(filename);
    if (in.fail())
    {
        cout << "Error opening file";
        exit (1);
    }
    lineNum=1;
    in.getline(buffer,200);
    p=0;
    len=strlen(buffer);
}
void close()
{
    in.close();
}
