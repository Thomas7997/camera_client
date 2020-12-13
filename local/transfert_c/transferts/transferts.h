#ifndef TRANSFERT
#define TRANSFERT

#define MAX_SIZE 30
#define MAX_FILES 10000

// LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonctions minimales
char ** commande_transfert(char ** historique1, char ** transferts, unsigned int *size, unsigned int *size1, char ** HistoRet);
char ** getHistorique (unsigned int * size);
char ** getTransferts (unsigned int * size);
void afficherList (char ** liste, unsigned int lines);
void freeHistorique(char ** historique);
void freeTransferts(char ** transferts);

// D'autres fonctions

#endif