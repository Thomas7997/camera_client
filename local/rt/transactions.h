#ifndef __TRANSACTIONS_H__
#define __TRANSACTIONS_H__

// LIBS

#include "mods.h"

// Fonctions

#include "errors.h"

unsigned int dossiers_to_list (char *** dossiers, char ** list, unsigned int nb_dossiers);
unsigned int listDir (char ** files, const char * path);
void removeFile (const char * path);

#endif