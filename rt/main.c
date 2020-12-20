#include <alchemy/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TASK_PRIO  99             /* Highest RT priority */
#define TASK_MODE  0  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */
#define TASK_PERM T_JOINABLE

// Checking demands
RT_TASK task_transfert_image_auto;
RT_TASK task_transfert_image_selected;
RT_TASK task_transfert_video_auto;

// Checking connections
RT_TASK task_wifi;

// Procedures
RT_TASK task_save_files_offline;
RT_TASK task_send_files_online;

unsigned int wifi_status = 0, image_selectionne, image_auto, video_auto;

void check_transfert_image_auto (void * arg) {
	while (1) {
		printf ("CHECK IMAGE AUTO ?\n");
		sleep(1);
	}
}

void check_transfert_video_auto (void * arg) {
	while (1) {
		printf ("CHECK VIDEO AUTO ?\n");
		sleep(1);
	}
}

void check_transfert_image_selected (void * arg) {
	while (1) {
		printf ("CHECK IMAGE SELECTIONNÉ ?\n");
		sleep(1);
	}
}

void check_wifi_status (void * arg) {
	while (1) {
		wifi_status = 0;
		// Emettre un messages

		sleep(2);
	}
}

void send_medias (void * arg) {
	while (1) {
		msg_wifi_n.data = "ON";
		msg_wifi_n.size = 32;
		printf("Envoyer médias : EN LIGNE\n");
		sleep(1);
	}
}

void save_medias (void * arg) {
	while (1) {
		msg_wifi_f.data = "ON";
		msg_wifi_f.size = 32;
		printf("Sauvegarder médias : HORS LIGNE\n");
		sleep(1);
	}
}

int main (void) {
	// Initialisations

	int result = 0;

	// Main tasks
	// Scripts de vérification
	result = rt_task_spawn (&task_transfert_image_auto, "IMAGE AUTO", 4096, 99, TASK_PERM, &check_transfert_image_auto, NULL);
	result = rt_task_spawn (&task_transfert_image_selected, "IMAGE SELECTED", 4096, 99, TASK_PERM, &check_transfert_image_selected, NULL);
	result = rt_task_spawn (&task_transfert_video_auto, "VIDEO AUTO", 4096, 99, TASK_PERM, &check_transfert_video_auto, NULL);
	result = rt_task_spawn (&task_wifi, "WIFI_STATUS", 4096, 99, 0, &check_wifi_status, NULL);
	
	// Scripts d'action
	result = rt_task_create (&task_save_files_offline, "SAVE MEDIAS", 4096, 99, TASK_PERM);
	result = rt_task_create (&task_send_files_online, "SEND MEDIAS", 4096, 99, TASK_PERM);

	if (wifi_status == 1) {
		result = rt_task_suspend (&task_send_files_online);
		result = rt_task_start (&task_save_files_offline, &send_medias, NULL);
	}

	else {
		result = rt_task_suspend(&task_save_files_offline);
		result = rt_task_start(&task_send_files_online, &save_medias, NULL);
	}

	while (1) {
		pause();
	}

	rt_task_delete(&task_transfert_video_auto);
	rt_task_delete(&task_transfert_image_selected);
	rt_task_delete(&task_transfert_image_auto);
	rt_task_delete(&task_wifi);
	rt_task_delete(&task_save_files_offline);
	rt_task_delete(&task_send_files_online);

	return 0;
}
