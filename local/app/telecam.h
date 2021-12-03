#ifndef __TELECAM_H__
#define __TELECAM_H__

// Main MODS
#include "mods.h"

// RT LIB
// #include <alchemy/task.h>

// New multithreading lib
#include <pthread.h>

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
#include "sd2.h" // Changed
#include "scripts.h"

// CONSTANTS
#define TASK_PRIO  99             /* Highest RT priority */
#define TASK_MODE  0  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */
#define TASK_PERM T_JOINABLE

// Function ids

pthread_t id_transfert_choice, id_save_files_offline, id_send_files_online, id_enable_transfert, id_apply_choice, id_wifi_transfert, id_send_model, id_notify_camera_status, id_wifi, id_manage_errors;

void enable_transfert_image_selection (void * arg);
void enable_transfert_image_auto (void * arg);
void enable_transfert_video_auto (void * arg);
void * check_wifi_status (void * arg);
void * check_transfert_choice (void * arg);
void * send_transferts_online (void * arg);
void send_medias (void * arg);
void save_medias (void * arg);
void camera_usb_connection (void * arg);
void * script_apply_choice (void * arg);
void reconnexion_usb (void * arg);
void free_usb (void * arg);
void camera_usb_connection_1 (void * arg);
void camera_usb_free_1(void * arg);
void send_model_fn (void * arg);
void * checkAndNotifyCameraStatus (void * arg);

// Nouvelle version de fonction
void generateError(int status);
void * manage_errors (void * arg);
void enable_transfert (void * arg);
void trigger_request_status (Status * status);
void write_choice (int choice);

// Carte SD
void cart_SD_mode (void * arg);
void check_sd_downloads (void * arg);
void check_sd_deletes (void * arg);
void sd_downloads (void * arg);
void sd_deletes (void * arg);
void sendOperationNotification (void * arg);
void sendDeleted (void * arg);
void sendDownloads (void * arg);

#endif