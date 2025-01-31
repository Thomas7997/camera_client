#ifndef __GLOBS_H__
#define __GLOBS_H__

// Variables globales
unsigned int wifi_status, transfert_choice, prevTransfertChoice, ttsk = 0, usb_connected = 0, nb_transferts; // 0 : none, 1 : image auto, 2 : image selection, 3 : video auto
static char ** transferts_send;
static Camera * camera;
static GPContext* context;
int status = 0, connected_once, prevStatus = 0, error = 0, res = 0, send_req_status = 0, reset = 0;
unsigned int result, command_usb_reconnexion, usb_freed, liste_captures_size;

// Stockage de fichiers
static char *** dossiers;
static char ** dirs_n;
static char ** files;
static unsigned int * dir_sizes;
static char ** images_list;
static char ** liste_captures;
static char ** transferts_tmp;
static char * model, * prevModel;
static char ** photos;
static int * transfert_tasks;
static char ** add;
static char ** supp;
static char ** cld_files;
static char ** downloads;
static char ** deletes;
static char ** downloaded_files;
static char ** deleted_files;

int send_model = 0;

int all_deleted, all_downloaded;

// status
int prevCamera_status, camera_status, prevOperation_status, operation_status, operation_mode, send_op_status;
char * operation_name;

typedef struct {
    int currentStatus;
    int prevStatus;
    int netStatus;
    int prevNetStatus;
    int send; // 0 ou 1
} Status;

Status messages;

#include "constants.h"

#endif