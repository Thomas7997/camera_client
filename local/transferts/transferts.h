#ifndef TRANSFERT
#define TRANSFERT

#define MAX_SIZE 20
#define MAX_FILES 10000

// LIBS
#include <stdio.h>
#include <stdlib.h>

// Fonctions minimales
char** getHistorique (unsigned int * size);
char** getTransferts (unsigned int * size);
void freeHistorique(char ** historique);
void freeTransferts(char ** transferts);

// D'autres fonctions

#endif