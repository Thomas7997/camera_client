#ifndef __SD_REFRESH_H__
#define __SD_REFRESH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

// CONSTANTES
#define TAILLE_NOM 100

// Fonctions
unsigned int read_cld (char ** cld);
void handleError(int status);
static int
recursive_directory(char *** dossiers, char ** dirs, Camera *camera, const char *folder, GPContext *context);
int get_sd_card_previews (char *** dossiers, unsigned int nb, Camera * camera, GPContext * context);
unsigned int dossiers_to_list (char *** dossiers, char ** list, unsigned int nb_dossiers);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
void declancher_transferer_hors_ligne(char ** transferts, unsigned int transferts_nb, GPContext * context, Camera * camera);
char * getName (char * buf, char * dossier);
int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);
void mirroir (char * buf, unsigned int n);
void diff_sd_list_refresh (char ** supp, char ** add, char ** cld_files, char ** sd_files, unsigned int cld_size, unsigned int sd_size, unsigned int * n_supp, unsigned int * n_add);

#endif