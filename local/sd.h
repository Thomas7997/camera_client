#ifndef __SD_H__
#define __SD_H__

// libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h>
#include <time.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

// CONSTANTES
#define TAILLE_NOM 100

// Fonctions
void handleError(int status);
static int
recursive_directory(char *** dossiers, char ** dirs, Camera *camera, const char *folder, GPContext *context);
int get_sd_card_previews (char *** dossiers, unsigned int nb, Camera * camera, GPContext * context);
unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
void declancher_transferer_hors_ligne(char ** transferts, unsigned int transferts_nb, GPContext * context, Camera * camera);
char * getName (char * buf, char * dossier);
int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);
void mirroir (char * buf, unsigned int n);

#endif