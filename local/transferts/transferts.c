#include "transferts.h"

char** getHistorique (unsigned int * size) {
    FILE * HISTORIQUE = fopen("historique.txt", "r");
    
    char ** historique = (char**) calloc(MAX_FILES, sizeof(char*));
    
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        historique[i] = (char*) calloc(MAX_SIZE, sizeof(char));
    }

    unsigned int x = 0;

    while (fgets(historique[x++], MAX_SIZE, HISTORIQUE));

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

    while (fgets(transferts[x++], MAX_SIZE, TRANSFERTS));

    *size = x;
    
    fclose(TRANSFERTS);
    return transferts;
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