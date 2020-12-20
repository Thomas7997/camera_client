#include "telecam.h"

int main (void) {
	// Initialisations

	int result = 0;

	// Main tasks
	// Scripts de vérification
	result = rt_task_spawn (&task_transfert_choice, "IMAGE AUTO", 4096, 99, TASK_PERM, &check_transfert_choice, NULL);
	result = rt_task_spawn (&task_wifi, "WIFI_STATUS", 4096, 99, TASK_PERM, &check_wifi_status, NULL);
    result = rt_task_spawn (&task_usb_connection, "USB CONNECTION", 4096, 99, TASK_PERM, &camera_usb_connection, NULL);

	// Scripts d'action
	result = rt_task_create (&task_save_files_offline, "SAVE MEDIAS", 4096, 99, TASK_PERM);
	result = rt_task_create (&task_send_files_online, "SEND MEDIAS", 4096, 99, TASK_PERM);
    result = rt_task_create (&task_enable_transfert_image_auto, "LANCER TRANSFERT IMAGE AUTO", 4096, 99, TASK_PERM);
    result = rt_task_create (&task_enable_transfert_image_selection, "LANCER TRANSFERT IMAGE SELECTION", 4096, 99, TASK_PERM);
    result = rt_task_create (&task_enable_transfert_video_auto, "LANCER TRANSFERT VIDEO AUTO", 4096, 99, TASK_PERM);

    if (usb_connected) {
        switch (transfert_choice) {
            case 0 :
                rt_task_suspend(&task_enable_transfert_image_auto);
                rt_task_suspend(&task_enable_transfert_image_selection);
                rt_task_suspend(&task_enable_transfert_video_auto);
            break;
            case 1 :
                rt_task_start(&task_enable_transfert_image_auto, &enable_transfert_image_auto, NULL);
                rt_task_suspend(&task_enable_transfert_image_selection);
                rt_task_suspend(&task_enable_transfert_video_auto);
            break;
            case 2 :
                rt_task_suspend(&task_enable_transfert_image_auto);
                rt_task_start(&task_enable_transfert_image_selection, &enable_transfert_image_selection, NULL);
                rt_task_suspend(&task_enable_transfert_video_auto);
            break;
            case 3 :
                rt_task_suspend(&task_enable_transfert_image_auto);
                rt_task_suspend(&task_enable_transfert_image_selection);
                rt_task_start(&task_enable_transfert_video_auto, &enable_transfert_video_auto, NULL);
            break;
            default : printf("Erreur\n");
            break;
        }

        if (wifi_status == 1) {
            result = rt_task_suspend(&task_send_files_online);
            result = rt_task_start(&task_save_files_offline, &send_medias, NULL);
        }

        else {
            result = rt_task_suspend(&task_save_files_offline);
            result = rt_task_start(&task_send_files_online, &save_medias, NULL);
        }
    }

	while (1) {
		pause();
	}

	rt_task_delete(&task_transfert_choice);
	rt_task_delete(&task_wifi);
	rt_task_delete(&task_save_files_offline);
	rt_task_delete(&task_send_files_online);
}

void check_transfert_choice (void * arg) {
	while (1) {
        transfert_choice = 2;
		printf ("CHECK TRANSFERT CHOICE\n");
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
		printf("Envoyer médias : EN LIGNE\n");
		sleep(1);
	}
}

void save_medias (void * arg) {
	while (1) {
		printf("Sauvegarder médias : HORS LIGNE\n");
		sleep(1);
	}
}

void enable_transfert_image_selection (void * arg) {
    while (1) {
        printf("TRANSFERT D'IMAGES SÉLECTIONNÉ LANCÉ\n");
        sleep(1);
    }
}

void enable_transfert_image_auto (void * arg) {
    while (1) {
        printf("TRANSFERT D'IMAGES AUTO LANCÉ\n");
        sleep(1);
    }
}

void enable_transfert_video_auto (void * arg) {
    while (1) {
        printf("TRANSFERT DE VIDEOS AUTO LANCÉ\n");
        sleep(1);
    }
}

void camera_usb_connection (void * arg) {
    context = sample_create_context();
    
    while (1) {
        printf("CONNECTION USB ...\n");

        if (transfert_choice > 0) {
            do {
                gp_camera_new (&camera);
                status = gp_camera_init(camera, context);
                handleError(status);

                if (status < 0) {
                    generateError(status);
                    gp_camera_exit(camera, context);
                    gp_camera_free(camera);
                }

                usleep(500000);
            } while (status != 0);
        }

        sleep(1);
    }
}