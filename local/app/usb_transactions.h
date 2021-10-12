#ifndef __USB_TRANSACTIONS_H__
#define __USB_TRANSACTIONS_H__

// LIBS
#include "mods.h"

// DONNÉES
#include "constants.h"

// Fonctions
#include "errors.h"
#include "scripts.h"

int reset_usb_dev (const char * filename);
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
int compare_file_historique (char * file, char ** historique, int lines);
int transferer_noms_auto (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
int transferer_nom_auto (char * nom, GPContext * context, Camera * camera);
int get_files (char ** files, Camera * camera, GPContext * context, unsigned int * x);
int recursive_directory(char ** files, Camera *camera, const char *folder, GPContext *context, unsigned int * x);
int delete_file (char ** files, char * name, Camera * camera, GPContext * context, char * err_name);
int download_file (char ** files, char * name, Camera * camera, GPContext * context, char * err_name);
static int selection_wait_event (Camera * camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename);
int control_selection (Camera * camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename);
int lsusb_find_camera (char * path);

#endif