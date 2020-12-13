#include "transferts.h"

char ** commande_transfert(char ** historique1, char ** transferts, unsigned int size, unsigned int size1, char ** HistoRet) {   
    int cmp=0;
    unsigned int  a=0;
    unsigned int ligne=0;
    unsigned int Nouveau[MAX_FILES] = {0};
    unsigned int index;
   
    for (unsigned int tran = 0;tran < size; tran++) {
        cmp = 0; 
        for(unsigned int his = 0;his < size1; his++) {
            if(strncmp(historique1[his], transferts[tran], strlen(historique1[his])) == 0) {
                cmp = 1;
                break;
            }
        }

        if(cmp == 0) {
            printf("TRANSFERER : %s\n", transferts[tran]);
            strcpy(HistoRet[a++], transferts[tran]);
            break;
        }
    }
}

void enlever_last_car (char * buffer) {
    unsigned int n = strlen(buffer)-1;
    buffer[n] = 0;
}

char** getHistorique (unsigned int * size) {
    FILE * HISTORIQUE = fopen("historique.txt", "r");
    
    char ** historique = (char**) calloc(MAX_FILES, sizeof(char*));
    
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        historique[i] = (char*) calloc(MAX_SIZE, sizeof(char));
    }

    unsigned int x = 0;

    while (fgets(historique[x++], MAX_SIZE, HISTORIQUE)) {
        //enlever_last_car(historique[x++]);
    }

    *size = x;
    
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

    while (fgets(transferts[x++], MAX_SIZE, TRANSFERTS)) {
        //enlever_last_car(transferts[x++]);
    }

    *size = x;
    
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