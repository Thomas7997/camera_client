#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TMaxL 100
#define TMax 1000

unsigned int read_file_list (char ** files, char ** lines, unsigned int nb, unsigned int start) {
    unsigned int line_size, x = 0, i, y;

    // Parser la liste de fichiers
    for (i = start; i < nb; i++) {
        if (lines[i][0] == '#') {
            for (y = 7; y <= 18; y++) {
                files[x][y-7] = lines[i][y];
            }
            x++;
        }

        else if (x != 0) break;
    }

    return x+1;
}

int select_dir (unsigned char * dossiers, FILE * File) {
    int k=0;
    int l=0;
    int r=0;
    char TAB1[TMaxL]="";
    char tmp[100]={0};
    int tab[TMax]={0};
    char code[TMax]="";
    char TAB2[10]="Le dossier";
    char TAB3[5]="aucun";
    File = fopen("files.txt", "r");
    if (File == NULL) {
        exit(0);
    }
   
    int rado=0;
    int longueur=0;
    for(int aze=0; aze<50000; aze++) {
        fgets(tmp,2000,File);
        while(tmp[rado]!=0) { 
            TAB1[longueur]=tmp[rado];
            longueur=longueur+1;
            rado=rado+1;
        }

        for(int zer=0; zer < 100; zer++) {
            tmp[zer]=0;
        }
        
        rado=0;
    }

    int x, c, same = 1;

    for (c = 0; c < longueur; c++) {
        same = 1;
        for (x = 0; x < 10; x++) {
            if (TAB1[c+x] != TAB2[x]) {
                same = 0;
                break;
            }
        }

        if (same == 1) {
            tab[k]=c+5;
            k=k+1;
        }
    }

    int g=0;
    for(int rty=0;rty<k;rty++) {
        g=tab[rty];
        for (int h=0; h<60; h++) {
            same = 1;
            for (int y = 0; y < 4; y++) {
                if(TAB1[g+h+y]!=TAB3[y]) {
                    same = 0;
                    break;
                }
            }

            if (same == 1) {
                tab[rty]=0;
            }
        }
    }
    int q=0;
    int t=0;

    char * result = calloc(100, sizeof(char));

    for(int s=0; s < k; s++) {
        if(tab[s]!=0) {
            // printf("%c",tab[s]);
            t=tab[s];
            for (int a=0; a<100; a++) {
                if (TAB1[t+a]=='/') {
                    while (TAB1[t+a+q]!='c') {
                        code[q]=TAB1[t+a+q];
                        q++;
                    }
                    goto label;
                }
            }
        }
    }

    label: {
        for (int i = 0; i < strlen(code)-3; i++) {
            result[i] = code[i];
        }
        
        printf("%s", result);
        printf("\n");
    }

    free(result);
        
    return 0;
}

int select_dirs (char ** dirs, FILE * FIC, unsigned int * lines) {
    // lines : lignes où se trouve les dossier contenant des images sur le fichier
}

int main (void) {
    FILE * FIC = fopen("files.txt", "r");

    unsigned int x = 0;
    char ** lines = calloc(1000, sizeof(char*));
    char ** files = calloc(1000, sizeof(char*));
    char ** dirs = calloc(100, sizeof(char*));
    unsigned int * l = calloc(100, sizeof(int));

    for (x = 0; x < 1000; x++) {
        lines[x] = calloc(100, sizeof(char));
        files[x] = calloc(100, sizeof(char));
    }

    for (x = 0; x < 100; x++) {
        dirs[x] = calloc(100, sizeof(char));
    }

    x = 0;

    while (fgets(lines[x++], 100, FIC));

    read_file_list(files, lines, x, 4);

    x = 0;

    while (files[x][0] != 0) {
        printf ("%s\n", files[x]);
        x++;
    }

    for (x = 0; x < 1000; x++) {
        free(files[x]);
        free(lines[x]);
    }
    
    for (x = 0; x < 100; x++) {
        free(dirs[x]);
    }

    free(lines);
    free(files);
    free(dirs);
    free(l);
    fclose(FIC);
    
    return 0;
}