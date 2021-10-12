#ifndef __SD_H__
#define __SD_H__

#include "mods.h"
#include "transactions.h"
#include "usb_transactions.h"

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context);
int sd_card_lecture_mode (char *** dossiers, char ** dirs_n, Camera * camera, GPContext * context);

#endif