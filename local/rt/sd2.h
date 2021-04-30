#ifndef __SD_H__
#define __SD_H__

#include "mods.h"
#include "transactions.h"
#include "usb_transactions.h"

int local_refresh(char ** supp, char ** add, unsigned int n_add, Camera * camera, GPContext * context);

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context);
void diff_sd_list_refresh (char ** supp, char ** add, char ** cld_files, char ** sd_files, unsigned int cld_size, unsigned int sd_size, unsigned int * n_add, unsigned int * n_supp);
int sd_card_lecture_mode (char ** files, Camera * camera, GPContext * context); // Called for first read from the desk app

int sd_refresh (char ** files, char ** supp, char ** add, char ** cld_files, Camera * camera, GPContext * context); // Called every refresh after the first read

#endif