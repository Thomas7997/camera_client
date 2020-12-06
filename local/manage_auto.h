#ifndef MANAGE_AUTO
#define MANAGE_AUTO

#define MAX_CAPTURES 10000
#define TAILLE_NOM 100

// STANDART
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

// GPHOTO
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

int generateError (int status);
int getCameraModel (Camera * cam);
void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
void getCaptureEvent(Camera * camera, GPContext * context);

#endif