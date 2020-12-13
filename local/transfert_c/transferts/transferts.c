#include "transferts.h"

char ** commande_transfert(char ** historique1, char ** transferts, unsigned int *size, unsigned int *size1, char ** HistoRet) {   
    int cmp=0;
    unsigned int  a=0;
    unsigned int ligne=0;
    unsigned int Nouveau[MAX_FILES] = {0};
    unsigned int index;
   
    for (unsigned int tran = 0;tran < *size; tran++) {
        cmp = 0;
        for(unsigned int his = 0;his < *size1; his++) {
            if(strcmp(historique1[his], transferts[trans]) == 0) {
                cmp = 1;
                break;
            }
        }

        if(cmp==0) {
            Nouveau[a]=tran;
            printf("TRANSFERER : %s\n", transferts[tran]);
            strcpy(HistoRet[a++], transferts[tran]);
            break;
        }
    }

    unsigned int NvT=(*size1)+1+a;
}

char** getHistorique (unsigned int * size) {
    FILE * HISTORIQUE = fopen("historique.txt", "r");
    
    char ** historique = (char**) calloc(MAX_FILES, sizeof(char*));
    
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        historique[i] = (char*) calloc(MAX_SIZE, sizeof(char));
    }

    unsigned int x = 0;

    while (fgets(historique[x++], MAX_SIZE, HISTORIQUE));

    *size = x-1;
    
    fclose(HISTORIQUE);
    return historique;
}

char** getTransferts (unsigned int * size) {
    FILE * TRANSFERTS = fopen("transferts.txt", "r");
    
    char ** transferts = (char**) calloc(MAX_FILES, sizeof(char*));
    
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        transferts[i] = (char*) calloc(MAX_SIZE, sizeof(char));
    }

    unsigned int x = 0;

    while (fgets(transferts[x++], MAX_SIZE, TRANSFERTS));

    *size = x-1;
    
    fclose(TRANSFERTS);
    return transferts;
}

void afficherList (char ** liste, unsigned int lines) {
    for (unsigned int x = 0; x < lines; x++) {
        printf("%s\n", liste[x]);
    }
}

void freeTransferts(char ** transferts) {
    for (unsigned int z = 0; z < MAX_FILES; z++) {
        free(transferts[z]);
    }
    free(transferts);
}

void freeHistorique(char ** historique) {
    for (unsigned int z = 0; z < MAX_FILES; z++) {
        free(historique[z]);
    }
    free(historique);
}