#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TMaxL 500000
#define TMax 1000

int main(void) {
    int k=0;
    int l=0;
    FILE * File;
    char TAB1[TMaxL];
    int tab[TMax]={0};
    char code[TMax];
    char TAB2[10]="Le dossier";
    char TAB3[5]="aucun";
    File = fopen("files.txt", "r" );
                if ( File == NULL ) {
                        printf ("File NULL\n"); 
			exit(0);
                                     }
     
        for(int i=0;i<TMaxL;i++)
  {
       
              TAB1[i]=fgetc(File);
  }
                    
 
  for(int c=0;c<TMaxL;c++)
 {
    if(TAB1[c]==TAB2[0])
    {
        if(TAB1[c+1]==TAB2[1])
    {
        if(TAB1[c+2]==TAB2[2])
        {
            if(TAB1[c+3]==TAB2[3])
            {
                if(TAB1[c+4]==TAB2[4])
                {if(TAB1[c+5]==TAB2[5])
                {if(TAB1[c+6]==TAB2[6])
                   {if(TAB1[c+7]==TAB2[7])

                   {
                    if(TAB1[c+8]==TAB2[8])
                     {
                        if(TAB1[c+9]==TAB2[9])    {
                            k=k+1;
                            tab[k]=c+10;
}}} }}}}}}}}

int g;
while(tab[l]!=0)
{g=tab[l];
for (int h=0;h<45;h++)
{
if(TAB1[g+h]==TAB3[0])
    {
        if(TAB1[g+1+h]==TAB3[1])
        {if(TAB1[g+h+2]==TAB3[2])
        {
            if(TAB1[g+h+3]==TAB3[3])
            {
               tab[l]=0; 
            }
        }
        }
    }

}
l=l+1;
}
int q=0;
int t=0;

for(int s=0;s<TMax;s++)
{
if(tab[s]!=0){
t=tab[s];
for (int a=0;a<10;a++)
{
if(TAB1[t+a]==0)				//<-------------------------						!!pb ici!!
{
    do
    {
      code[q]=TAB1[t+a+q];
       q=q+1;
    }while(TAB1[t+a+q]!='c');
}}}}
    for(int u=0;u<1000;u++) {
        printf("%c",code[u]);
    }
    fclose(File);
        
    return 0;}

