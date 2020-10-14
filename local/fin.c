#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TMaxL 150000
#define TMax 10000

int main (void) {
    int k=0;                                     //nb de fois ou on detecte les mots "le dossier"
    int l=0;int r=0;
    FILE * File;
    char TAB1[TMaxL] = "";                      //   va contenir tt le file.txt
    char tmp[100]={0};
    int tab[TMax]={0};                      //liée  a k contient les indice de ou les mots "le dossier" sont dans le TAB1
    char code[TMax]="";                    //contient les nom des dissier separer par "+%+"
    char code1[500]="";                    
    char TAB2[10]="Le dossier";
    char TAB3[5]="aucun";
    File = fopen("files.txt", "r" );
    if (File == NULL) {
        exit(0);
    }
  
    int rado = 0;
    int longeur = 0;
    // boucle for pour mettre dans TAB1 
    for(int aze=0; aze<50000; aze++) {
        fgets(tmp,2000,File);
        while(tmp[rado] != 0) { 
            TAB1[longeur] = tmp[rado];
            longeur = longeur+1;
            rado = rado + 1;
        }

        for(int zer=0; zer<100; zer++) {
            tmp[zer] = 0;
        }

        rado=0;
    }

    // compare "le dossier" et TAB1[X] parcour tout TAB1

    for(int c = 0;c < longeur; c++) {
        if(TAB1[c]==TAB2[0]) {
        if(TAB1[c+1]==TAB2[1]) {
        if(TAB1[c+2]==TAB2[2]) {
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
                           
                            tab[k]=c+3;                   
                             k=k+1;
    }}} }}}}}}}}

    int g=0;
    for(int rty=0;rty<k;rty++) {
        g=tab[rty];
        for (int h=0;h<60;h++) {
            if(TAB1[g+h]==TAB3[0]) {
                if(TAB1[g+1+h]==TAB3[1]) {
                    if(TAB1[g+h+2]==TAB3[2]) {
                            if(TAB1[g+h+3]==TAB3[3]) {
                                tab[rty]=0; 
                            }
                        }
                    }
                }

            }
        }
    int q=0;
    int t=0;
    int qsd=0;

    for(int s=0;s<k;s++) {
        if(tab[s] != 0) {
            printf("%d  ,,",tab[s]);
        t = tab[s];
        qsd=0;
        for (int a=0;a<15;a++) {
            if(TAB1[t+a]=='/') {
            while(TAB1[t+a+qsd]!=' ') { 
            code[q]=TAB1[t+a+qsd];
                
                qsd++;  
                q=q+1;
            }
        }
    }  
 
    code[q]='+';code[q+1]='%';code[q+2]='+';q=q+3; }}
    t=0;
    int klm=0;
    int fdj=0;
    for(int jkl=0;jkl<k;jkl++)
    {
    if(tab[jkl]!=0){printf("(1) %d  ,,",tab[jkl]);
    klm=tab[jkl];
    for (int hjk=0;hjk<100;hjk++)
    {	
    if(TAB1[klm+hjk]=='#')
                
    {  while (TAB1[klm+hjk+fdj]!='I')
    {   
        code1[t]=TAB1[klm+hjk+fdj];

    fdj++;
    t++;}
    }fdj=0;}}}

    for(int u=0;u<10000;u++) {
        printf("%c",code1[u]);
    }

 fclose( File );
        
    return 0;}





 

