#ifndef __SELECTION_OPTIMALE_H__
#define __SELECTION_OPTIMALE_H__

// LIBS

#include "mods.h"

// Fonctions

#include "errors.h"
#include "transactions.h"
#include "usb_transactions.h"

int selection_optimale (Camera * camera, GPContext * context, char ** transferts, unsigned int * transferts_nb, unsigned int * command, unsigned int * freed, char *** dossiers, char ** dirs_n, unsigned int * dir_sizes, char ** files, char ** images_list);

#endif