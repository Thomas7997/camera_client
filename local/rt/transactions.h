#ifndef __TRANSACTIONS_H__
#define __TRANSACTIONS_H__

// LIBS

#include "mods.h"

// Fonctions

#include "errors.h"

unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files);

#endif