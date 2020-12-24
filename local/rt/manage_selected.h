#ifndef MANAGE_SELECTED_H
#define MANAGE_SELECTED_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <unistd.h>
#include <ctime>
#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <time.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

#include "errors.h"

#define MAX 80
#define MAX_CAPTURES 10000
#define TAILLE_NOM 100
#define MIN_DIRS 10
#define NB_PARTS 2
#define PART_NB 49

struct _Dossier {
    char * title;
    char ** images;
    unsigned int nb_images;
    unsigned int ref_line;
};

typedef struct _Dossier Dossier;

// Création d'une fonction pour ne sélectionner que les X derniers éléments de la liste de photos
void cut_list(char ** files, unsigned int size, char ** newList);

void mirroir (char * buf, unsigned int n);
int getCameraModel (Camera * cam);
char * getName (char * buf, char * dossier);
void clearBufLast (char * buf, unsigned int len, unsigned int nb);
void vider_buffer (char *buffer);
void afficher_tab2 (char ** tab);
void transform_noms (char ** liste, char ** nouvelleListe, int size);
void linearize (char *base, char **lines);
int compare_file_historique (char * file, char ** historique, int lines);
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera, int wifiStatus);
int getPlacements(int * rating, char * dir, char * file, GPContext * context, Camera * camera);
int sauvegarder_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
void enlever_last_car(char *chaine);
void select_medias ();
void getFiles (void);
void findPathIndex(char ** files, char * name, char * path);
unsigned int parseRating (char * line);
void parseRatings (int * ratings, char ** lines, unsigned int size);
int eachFileRating (char *** dossiers, char ** dirs, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs, unsigned int * transferts_nb, Camera * camera, GPContext * context);
int eachFileRating_1 (char ** files, char ** transferts, unsigned int files_nb, unsigned int * transferts_nb, Camera * camera, GPContext * context);
int eachFileRating_2 (char ** files, char ** transferts, unsigned int files_nb, unsigned int * transferts_nb, Camera * camera, GPContext * context);
void declancher_transferer_hors_ligne(char ** transferts, unsigned int transferts_nb, GPContext * context, Camera * camera);

#endif