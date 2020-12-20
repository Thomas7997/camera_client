#ifndef __TRANSFERTS_H__
#define __TRANSFERTS_H__

#include "net.h"
#include "auto.h"

void supp_last_char (char * buffer);
int transferts (Camera * camera, GPContext * context, char ** transferts, unsigned int transfert_choice);

#endif