#ifndef __TELECAM_H__
#define __TELECAM_H__

// Main MODS
#include "mods.h"

// RT LIB
#include <alchemy/task.h>

#include "globs.h"

// MODULES
#include "transferts.h"
#include "photo_auto.h"
#include "net.h"
#include "auto.h"
#include "errors.h"
#include "selection_optimale.h"
#include "usb_transactions.h"
#include "model.h"
#include "video_auto.h"
#include "sd2.h"

// CONSTANTS
#define TASK_PRIO  99             /* Highest RT priority */
#define TASK_MODE  0  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */
#define TASK_PERM T_JOINABLE

// Reconnexion
RT_TASK task_reconnexion, task_free_usb;

// Checking demands
RT_TASK task_transfert_choice;

// Checking connections
RT_TASK task_wifi, task_usb_connection, task_manage_errors;

// Procedures
RT_TASK task_save_files_offline, task_send_files_online, task_enable_transfert_image_auto, task_enable_transfert_image_selection, task_enable_transfert_video_auto, task_enable_transfert;

// Général
RT_TASK task_apply_choice, task_wifi_transfert;

void enable_transfert_image_selection (void * arg);
void enable_transfert_image_auto (void * arg);
void enable_transfert_video_auto (void * arg);
void check_wifi_status (void * arg);
void check_transfert_choice (void * arg);
void send_transferts_online (void * arg);
void send_medias (void * arg);
void save_medias (void * arg);
void camera_usb_connection (void * arg);
void script_apply_choice (void * arg);
void reconnexion_usb (void * arg);
void free_usb (void * arg);
void camera_usb_connection_1 (void * arg);
void camera_usb_free_1(void * arg);

// Nouvelle version de fonction
void generateError(int status);
void manage_errors (void * arg);
void enable_transfert (void * arg);
void trigger_request_status (Status * status);

// Carte SD
void cart_SD_mode (void * arg);

#endif