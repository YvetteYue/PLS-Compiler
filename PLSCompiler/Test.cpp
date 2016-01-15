#include "header.h"
#include "def.h"
#include<iostream>
using namespace std;
void test()
{
    cout<<"atab "<<endl;
    for(int i=0; i<=sx; i++)
    {
        cout<<i<<"..."<<stab[i]<<endl;
    }
    cout<<" elsize sz   typ "<<endl;
    for(int i=0; i<=a; i++)
    {
        //  printf("%5d",atab[i].elsize);
        printf("%5d",atab[i].sz);
        printf("%10s\n",Typs[atab[i].typ]);
    }
    cout<<"tab"<<endl;
    cout<<" num|  name|  adr|  lev|  link|   normal|   obj|     reff|    typ|"<<endl;
    for(int i=0; i<=t; i++)
    {
        testtab(i);
    }
    cout<<"display"<<endl;
    for(int i=0; i<=level; i++)
    {
        testdisplay(i);
    }
    cout<<"last   lastpar    psize     vsize "<<endl;
    for(int i=0; i<=b; i++)
    {
        testbtab(i);
    }
}
void testtab(int i)
{
    cout<<i<<"|";
    cout<<"       "<<tab[i].name<<"|";
    printf("%5d|",tab[i].adr);
    printf("%5d|",tab[i].lev);
    printf("%5d|",tab[i].link);
    printf("%5d|",tab[i].normal);
    printf("%10s|",Objs[tab[i].obj]);
    printf("%5d|",tab[i].reff);
    printf("%10s|\n",Typs[tab[i].typ]);
}
void testdisplay(int i)
{
    printf("%10d\n",display[i]);
}
void testbtab(int i)
{
    cout<<" "<<btab[i].last<<" "<<btab[i].lastpara<<" "<<btab[i].psize<<" "<<btab[i].vsize<<endl;
}
