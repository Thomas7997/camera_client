#ifndef __SCRIPTS_H__
#define __SCRIPTS_H__

// LIBS

#include "mods.h"
#include "constants.h"

// Fonctions

char * getName (char * buf, char * dossier);
void enlever_last_car(char *chaine);
void mirroir (char * buf, unsigned int n);
void clearList (char ** list);
void clearStr (char * str);
void dislpayList(char ** list);
int find_dir_filename (char ** files, const char * name, char * dir);
void operation_finished (const char * path, const char * name);

#endif