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

#define TYPE_JPG 0
#define TYPE_MKV 1
#define TYPE_DOSSIER 2

#define REL_FILS 1
#define REL_FRERE 2

#define MAX_FOLDER_LEN 100
#define MAX_FILE_LEN 100

void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();

void mirroir (char * buf, unsigned int n);
char * getName (char * buf, char * dossier);
int get_sd_card_previews (char *** dossiers, unsigned int nb, Camera * camera, GPContext * context);
// int sd_card_lecture_mode (Camera * camera, GPContext * context);
static int
recursive_directory(char *** dossiers, Camera *camera, const char *folder, GPContext *context, unsigned int * x);

#endif