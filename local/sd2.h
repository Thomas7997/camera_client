#ifndef __SD_H__
#define __SD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

// typedef struct _Fichiers {
//     char nom[100];
//     char type[10];
//     struct _Fichiers * suiv;
// } Fichiers;

#define TYPE_JPG 0
#define TYPE_MKV 1
#define TYPE_DOSSIER 2

#define REL_FILS 1
#define REL_FRERE 2

#define MAX_FOLDER_LEN 100
#define MAX_FILE_LEN 100

typedef struct _Fichiers {
    char nom[100];
    int type;
    struct _Fichiers * fils;
    struct _Fichiers * frere;
} Fichiers;

int x = -1, dir_nb = 0;

void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();

Fichiers* insertenqueue_Fichiers (Fichiers * liste, const char * valeur, int type, int rel);
// Dossiers* insertenqueue_Dossiers (Dossiers * liste, const char * valeur, Fichiers * fichiers, Dossiers * fils);
Fichiers * new_fichiers_list (void);
// Dossiers * new_dossiers_list (void);
void free_fichiers (Fichiers * fichiers);
// void free_dossiers (Dossiers * dossiers);
// int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context);
// int sd_card_lecture_mode (Camera * camera, GPContext * context);
void afficher_fichiers_liste (Fichiers * fichiers);
// int get_files_and_dirs (char *** dirs_b, Camera * camera, GPContext * context);
// int get_folders (Camera * camera, GPContext * context);
static int
recursive_directory(char *** dossiers, char ** dirs, Camera *camera, const char *folder, GPContext *context);
// void afficher_fichiers_liste (Fichiers * fichiers);
// void afficher_carte_sd_liste (Dossiers * dossiers);

#endif