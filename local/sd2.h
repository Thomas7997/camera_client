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

typedef struct _Fichiers {
    char * nom;
    char * type;
    struct _Fichiers * suiv;
} Fichiers;

typedef struct _Dossiers {
    char * nom;
    Fichiers * fichiers;
    struct _Dossiers * fils;
    struct _Dossiers * suiv;
} Dossiers;

void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();

Fichiers* insertenqueue_Fichiers (Fichiers * liste, const char * valeur, const char * type);
Dossiers* insertenqueue_Dossiers (Dossiers * liste, const char * valeur, Fichiers * fichiers, Dossiers * fils);
Fichiers * new_fichiers_list (void);
Dossiers * new_dossiers_list (void);
void free_fichiers (Fichiers * fichiers);
void free_dossiers (Dossiers * dossiers);
// int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context);
// int sd_card_lecture_mode (Camera * camera, GPContext * context);
void afficher_fichiers_liste (Fichiers * fichiers);
void afficher_carte_sd_liste (Dossiers * dossiers);
int get_files_and_dirs (Dossiers ** dirs_b, Camera * camera, GPContext * context);
void afficher_fichiers_liste (Fichiers * fichiers);
void afficher_carte_sd_liste (Dossiers * dossiers);

#endif