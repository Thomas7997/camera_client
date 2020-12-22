#ifndef __USB_TRANSACTIONS_H__
#define __USB_TRANSACTIONS_H__

// LIBS
#include "mods.h"

// Fonctions
#include "errors.h"

int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);

#endif