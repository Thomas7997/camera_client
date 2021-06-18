#ifndef __USB_TRANSACTIONS_H__
#define __USB_TRANSACTIONS_H__

// LIBS
#include "mods.h"

// DONNÉES
#include "constants.h"

// Fonctions
#include "errors.h"
#include "scripts.h"

int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);
// Pour l'envoi
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
int compare_file_historique (char * file, char ** historique, int lines);
int transferer_noms_auto (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
int transferer_nom_auto (char * nom, GPContext * context, Camera * camera);
int get_files (char ** files, Camera * camera, GPContext * context, unsigned int * x);
int
recursive_directory(char ** files, Camera *camera, const char *folder, GPContext *context, unsigned int * x);
int delete_file (char * name, Camera * camera, GPContext * context);
int download_file (char ** files, char * name, Camera * camera, GPContext * context);

#endif