#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TMaxL 150000
#define TMax 10000

void clearBufLast (char * buf, unsigned int len, unsigned int nb) {
    int i;

    for (i = len; i >= len - nb; i--)
        buf[i] = 0;
    return;
}

unsigned int load_dossiers (char ** dossiers, const char * path, unsigned int * refs) {
    int k=0; //nb de fois ou on detecte les mots "le dossier"
    int l=0;
    int r=0;
    char TAB1[TMaxL] = ""; // va contenir tt le file.txt
    char tmp[100]={0};
    int tab[TMax]={0}; //liée  a k contient les indice de ou les mots "le dossier" sont dans le TAB1
    char code[TMax]=""; //contient les noms des dossiers separés par "+%+"
    char code1[500]="";                    
    char TAB2[10]= "Le dossier";
    char TAB3[5]="aucun";
    unsigned int * ks = calloc(10, sizeof(unsigned int));

    FILE * File;
    File = fopen(path, "r");
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
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int xk = 0;

    for(int s=0;s<k;s++) {
        if(tab[s] != 0) {
            ks[xk++] = s;
            strcpy(code, "");
            // printf("%d  ,,",tab[s]);
            t = tab[s];
            qsd=0;
            for (int a=0;a<15;a++) {
                if(TAB1[t+a]=='/') {
                    y = 0;
                    while(TAB1[t+a+qsd+2]!=' ') { 
                        code[q]=TAB1[t+a+qsd];
                        printf ("%c", code[q]);
                        dossiers[x][y++] = code[q];
                        qsd++;  
                        q=q+1;
                    }
                    printf ("1\n");
                    x++;
                }
            }

            // code[q]='+';code[q+1]='%';code[q+2]='+';q=q+3;
        }
    }

    t=0;
    int klm=0;
    int fdj=0;
    for(int jkl=0;jkl<k;jkl++) {
        if(tab[jkl]!=0) {
            printf("%d\n",tab[jkl]);
            klm=tab[jkl];
            for (int hjk=0;hjk<100;hjk++) {	
                if(TAB1[klm+hjk]=='#') {
                    while (TAB1[klm+hjk+fdj]!='I') {   
                        code1[t]=TAB1[klm+hjk+fdj];
                        fdj++;
                        t++;
                    }
                }
                fdj=0;
            }
        }
    }

    // for(int u=0;u<10000;u++) {
    //     printf("%c",code1[u]);
    // }


    unsigned int u = 0;
    char * number = calloc(5, sizeof(char));

    x = 0;

    while (code1[u] != 0) {
        // printf ("%c", code1[u]);
        strcpy(number, "");
        if (code1[u-1] == '#') {
            y = 0;
            while (code1[u] != ' ') {
                number[y++] = code1[u];
         void load_dossiers (char ** dossiers, const char * path, unsigned int * refs) {
    int k=0; //nb de fois ou on detecte les mots "le dossier"
    int l=0;
    int r=0;
    char TAB1[TMaxL] = ""; // va contenir tt le file.txt
    char tmp[100]={0};
    int tab[TMax]={0}; //liée  a k contient les indice de ou les mots "le dossier" sont dans le TAB1
    char code[TMax]=""; //contient les noms des dossiers separés par "+%+"
    char code1[500]="";                    
    char TAB2[10]= "Le dossier";
    char TAB3[5]="aucun";
    unsigned int * ks = calloc(10, sizeof(unsigned int));

    FILE * File;
    File = fopen(path, "r");
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
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int xk = 0;

    for(int s=0;s<k;s++) {
        if(tab[s] != 0) {
            ks[xk++] = s;
            strcpy(code, "");
            // printf("%d  ,,",tab[s]);
            t = tab[s];
            qsd=0;
            for (int a=0;a<15;a++) {
                if(TAB1[t+a]=='/') {
                    y = 0;
                    while(TAB1[t+a+qsd+2]!=' ') { 
                        code[q]=TAB1[t+a+qsd];
                        printf ("%c", code[q]);
                        dossiers[x][y++] = code[q];
                        qsd++;  
                        q=q+1;
                    }
                    printf ("1\n");
                    x++;
                }
            }

            // code[q]='+';code[q+1]='%';code[q+2]='+';q=q+3;
        }
    }

    t=0;
    int klm=0;
    int fdj=0;
    for(int jkl=0;jkl<k;jkl++) {
        if(tab[jkl]!=0) {
            printf("%d\n",tab[jkl]);
            klm=tab[jkl];
            for (int hjk=0;hjk<100;hjk++) {	
                if(TAB1[klm+hjk]=='#') {
                    while (TAB1[klm+hjk+fdj]!='I') {   
                        code1[t]=TAB1[klm+hjk+fdj];
                        fdj++;
                        t++;
                    }
                }
                fdj=0;
            }
        }
    }

    // for(int u=0;u<10000;u++) {
    //     printf("%c",code1[u]);
    // }


    unsigned int u = 0;
    char * number = calloc(5, sizeof(char));

    x = 0;

    while (code1[u] != 0) {
        // printf ("%c", code1[u]);
        strcpy(number, "");
        if (code1[u-1] == '#') {
            y = 0;
            while (code1[u] != ' ') {
                number[y++] = code1[u];
                // printf ("%c", code1[u]);
                u++;
            }

            sscanf(number, "%d", &refs[x]);

            // Calcul
            refs[x] = refs[x] + ks[x];
            x++;
        }
        
        u++;
    }

    free(number);
    free(ks);
    fclose(File);
}       // printf ("%c", code1[u]);
                u++;
            }

            sscanf(number, "%d", &refs[x]);

            // Calcul
            refs[x] = refs[x] + ks[x];
            x++;
        }
        
        u++;
    }

    free(number);
    free(ks);
    fclose(File);

    return x+1;
}

int main (void) {
    char ** dossiers = calloc(10, sizeof(char*));
    unsigned int * refs = calloc(10, sizeof(unsigned int));

    for (int i = 0; i < 10; i++) {
        dossiers[i] = calloc(100, sizeof(char));
    }

    load_dossiers(dossiers, "files.txt", refs);
    unsigned int x = 0;

    while (dossiers[x][0] != 0) {
        printf ("%s\n", dossiers[x++]);
    }

    x = 0;

    while (refs[x] != 0) {
        printf("%d\n", refs[x++]);
    }

    for (int i = 0; i < 10; i++) {
        free(dossiers[i]);
    }

    free(dossiers);
    free(refs);
        
    return 0;
}
