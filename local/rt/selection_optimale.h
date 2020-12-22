#ifndef __SELECTION_OPTIMALE_H__
#define __SELECTION_OPTIMALE_H__

// LIBS

#include "mods.h"

// Fonctions

#include "errors.h"
#include "transactions.h"
#include "usb_transactions.h"

int selection_optimale (Camera * camera, GPContext * context, unsigned int * command, unsigned int * freed);

#endif