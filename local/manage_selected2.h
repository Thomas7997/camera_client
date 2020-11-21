#ifndef MANAGE_SELECTED2_H
#define MANAGE_SELECTED2_H

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

#define MAX 80
#define MAX_CAPTURES 10000
#define TAILLE_NOM 100
#define TMaxL 150000
#define TMax 100
#define MIN_DIRS 10
#define NB_PARTS 2

struct _Dossier {
    char * title;
    char ** images;
    unsigned int nb_images;
    unsigned int ref_line;
};

typedef struct _Dossier Dossier;

void mirroir (char * buf, unsigned int n);
int generateError (int status);
int getCameraModel (Camera * cam);
char * getName (char * buf, char * dossier);
void clearBufLast (char * buf, unsigned int len, unsigned int nb);
void send_request (char *name);
void vider_buffer (char *buffer);
void afficher_tab2 (char ** tab);
// unsigned int load_dossiers (char ** dossiers, const char * path, unsigned int * refs);
void transform_noms (char ** liste, char ** nouvelleListe, int size);
void linearize (char *base, char **lines);
int compare_file_historique (char * file, char ** historique, int lines);
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera, int wifiStatus);
int getPlacements(int * rating, char * dir, char * file, GPContext * context, Camera * camera);
int sauvegarder_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera);
void enlever_last_car(char *chaine);
void select_medias ();
void getFiles (void);
unsigned int parseRating (char * line);
void parseRatings (int * ratings, char ** lines, unsigned int size);
//unsigned int read_dir_list (char ** dossiers, unsigned int * refs);
//unsigned int read_file_list (char ** files, char ** lines, unsigned int nb, unsigned int starts);
int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);
int eachFileRating (char *** dossiers, char ** dirs, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs, unsigned int * transferts_nb, Camera * camera, GPContext * context);
int eachFileRating_1 (char ** files, char ** transferts, unsigned int files_nb, unsigned int * transferts_nb, Camera * camera, GPContext * context);
void handleError(int status);
void send_status_request (int status);
unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
void declancher_transferer_hors_ligne(char ** transferts, unsigned int transferts_nb, GPContext * context, Camera * camera);

#endif