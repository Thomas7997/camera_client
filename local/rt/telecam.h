#ifndef __TELECAM_H__
#define __TELECAM_H__

// MODULES
#include "transferts.h"
#include "photo_auto.h"
#include "net.h"
#include "auto.h"

// RT LIB
#include <alchemy/task.h>

// CONSTANTS
#define TASK_PRIO  99             /* Highest RT priority */
#define TASK_MODE  0  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */
#define TASK_PERM T_JOINABLE

// Checking demands
RT_TASK task_transfert_choice;

// Checking connections
RT_TASK task_wifi, task_usb_connection;

// Procedures
RT_TASK task_save_files_offline, task_send_files_online, task_enable_transfert_image_auto, task_enable_transfert_image_selection, task_enable_transfert_video_auto;

unsigned int wifi_status, transfert_choice, usb_connected, nb_transferts; // 0 : none, 1 : image auto, 2 : image selection, 3 : video auto
static char ** transferts_s;
static Camera * camera;
static GPContext* context;
int status;

void enable_transfert_image_selection (void * arg);
void enable_transfert_image_auto (void * arg);
void enable_transfert_video_auto (void * arg);
void check_wifi_status (void * arg);
void check_transfert_choice (void * arg);
void send_medias (void * arg);
void save_medias (void * arg);
void camera_usb_connection (void * arg);

#endif