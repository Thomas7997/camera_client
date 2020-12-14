// Header déstiné au développement des fonctions utilisées dans les scripts de transferts vidéo et photo
#ifndef __AUTO_H__
#define __AUTO_H__

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

#define MAX 80
#define MAX_CAPTURES 10000
#define TAILLE_NOM 100
#define MIN_DIRS 10
#define NB_PARTS 2
#define PART_NB 50

unsigned int filterPhotos (char ** photos_l, char ** files_l, unsigned int files_n);
unsigned int filterVideos (char ** videos_l, char ** files_l, unsigned int files_n);
unsigned int getImageNumber (char * buffer);
int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context);
unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files);
unsigned int save_clist_slist(char ** liste_captures, char ** files, unsigned int files_nb, unsigned int nb_list);
void afficher_liste(char ** transferts, unsigned int ret_comp);
int compareFilesLists(char ** transferts, char ** files, char ** liste_captures, int files_nb, int nb_list, unsigned int * indexs);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
int generateError (int status);
void handleError(int status);
void mirroir (char * buf, unsigned int n);
unsigned int getVideoDatas (char *** dossiers, char ** dirs_n, char ** photos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes);
unsigned int getPhotoDatas (char *** dossiers, char ** dirs_n, char ** photos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes);

#endif