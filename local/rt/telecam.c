#include "telecam.h"

// Actuellement le switch de mode génère un bug sur lequel il faut travailller
int main (void) {
	// Initialisations
    dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    images_list = (char**) calloc(PART_NB, sizeof(char*));
    dir_sizes = (unsigned int*) calloc(MIN_DIRS, sizeof(unsigned int*));
    liste_captures = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    transferts_tmp = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    model = (char*) calloc(MAX_CAPTURES, sizeof(char));
    photos = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    prevStatus = 0;
    status = 0;

    for (unsigned int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        images_list[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }
    
    transferts_send = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        transferts_send[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts_tmp[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        photos[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    context = sample_create_context();

    usb_freed = 1;
    command_usb_reconnexion = 1;
    connected_once = -1;

	// Main tasks

    // Scripts de vérification
	result = rt_task_spawn (&task_transfert_choice, "TRANSFERT CHOICE", 4096, 99, TASK_PERM, &check_transfert_choice, NULL);
	// result = rt_task_spawn (&task_wifi, "WIFI_STATUS", 4096, 99, TASK_PERM, &check_wifi_status, NULL);
    // result = rt_task_spawn (&task_manage_errors, "MANAGE ERRORS", 4096, 99, TASK_PERM, &manage_errors, NULL);

	// Scripts d'action
	result = rt_task_create (&task_save_files_offline, "SAVE MEDIAS", 4096, 99, TASK_PERM);
	result = rt_task_create (&task_send_files_online, "SEND MEDIAS", 4096, 99, TASK_PERM);

    result = rt_task_create (&task_enable_transfert, "LANCER TRANSFERT IMAGE AUTO", 10000, 99, TASK_PERM);

    // MAIN SCRIPTS
    result = rt_task_spawn (&task_apply_choice, "APPLIQUER LE CHOIX DE L'UTILISATEUR", 4096, 99, TASK_PERM, &script_apply_choice, NULL);
    result = rt_task_spawn (&task_wifi_transfert, "APPLIQUER LE TRANSFERT WIFI AUTONOME", 4096, 99, TASK_PERM, &send_transferts_online, NULL);

    // Lancement des tâches et suspension de certaines

    while (1) {
		pause();
	}

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        free(transferts_send[e]);
        free(transferts_tmp[e]);
        free(photos[e]);
    }

    free(transferts_send);

    free_usb(NULL);
    
	rt_task_delete(&task_transfert_choice);
	rt_task_delete(&task_wifi);
	rt_task_delete(&task_save_files_offline);
	rt_task_delete(&task_send_files_online);
    rt_task_delete(&task_apply_choice);
    rt_task_delete(&task_wifi_transfert);
    rt_task_delete(&task_manage_errors);

    for (int d = 0; d < MIN_DIRS; d++) {
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            free(dossiers[d][dy]);
        }
        free(dossiers[d]);
        free(dirs_n[d]);
    }

    for (int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        free(files[i]);
        free(liste_captures[i]);
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        free(images_list[i]);
    }

    free(dirs_n);
    free(dir_sizes);
    free(dossiers);
    free(files);
    free(images_list);
    free(liste_captures);
    free(transferts_tmp);
    free(model);
    free(photos);
}

void check_transfert_choice (void * arg) {
	while (1) {
        FILE * TRANSFERT_CHOICE = fopen("../data/tmp/transfert_choice.txt", "r"); // Changera de chemin
        fscanf(TRANSFERT_CHOICE, "%d", &transfert_choice);
        prevTransfertChoice = transfert_choice;
		printf ("CHECK TRANSFERT CHOICE\n");
        fclose(TRANSFERT_CHOICE);
        usleep(500000);
	}
}

void check_wifi_status (void * arg) {
	while (1) {
        FILE * WIFI = fopen("../data/tmp/wifi_status.txt", "r");
		fscanf(WIFI, "%d", &wifi_status);
		
        // Emettre un message
        printf ("wifi_status : %d\n", wifi_status);

        fclose(WIFI);
		usleep(500000);
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

// Fonctionne
void enable_transfert_image_selection (void * arg) {
    while (1) {
        printf("TRANSFERT D'IMAGES SÉLECTIONNÉ LANCÉ\n");

        reset = 1;

        camera_usb_connection_1 (NULL);

        // Avant de récupérer le modèle
        // while (usb_connected != 1) {
        //     usleep(500);
        // }

        status = getModel(model, camera, &send_model);

        if (status != 0) {
            generateError(status);
            continue;
        }

        printf("%s\n", model);

        status = selection_optimale (camera, context, transferts_send, &nb_transferts, &command_usb_reconnexion, &usb_freed, dossiers, dirs_n, dir_sizes, files, images_list, transferts_tmp, photos);

        if (status < 0) {
            generateError(status);
            continue;
        }

        camera_usb_free_1 (NULL);

        if (transfert_choice != 2) return;
    }
}

// Fonctionne
void enable_transfert_image_auto (void * arg) {   
    int nb_tours = 0;

    while (1) {
        printf("TRANSFERT D'IMAGES AUTO LANCÉ\n");

        reset = 1;

        camera_usb_connection_1 (NULL);

        while (usb_connected != 1) {
            usleep(500);
        }

        // Attention aux code d'erreurs des requêtes http
        status = getModel(model, camera, &send_model);

        if (status != 0) {
            generateError(status);
            continue;
        }

        printf("%s\n", model);

        status = photo_auto(camera, context, transferts_send, &nb_transferts, &nb_tours, liste_captures, &liste_captures_size);

        if (status < 0) {
            handleError(status);
            generateError(status);
            continue;
        }

        camera_usb_free_1(NULL);

        if (transfert_choice != 1) return;

        printf("tours : %d\n", nb_tours++);
        usleep(500000);
    }
}

// A travailler
void enable_transfert_video_auto (void * arg) {
    int nb_tours = 0;

    while (1) {
        printf("TRANSFERT DE VIDEOS AUTO LANCÉ\n");

        reset = 1;

        camera_usb_connection_1(NULL);

        status = getModel(model, camera, &send_model);

        if (status < 0) {
            generateError(status);
            continue;
        }

        printf("%s\n", model);

        status = video_auto(camera, context, transferts_send, &nb_transferts, &nb_tours, liste_captures, &liste_captures_size);

        if (status < 0) {
            generateError(status);
            continue;
        }

        camera_usb_free_1(NULL);

        if (transfert_choice != 3) return;

        printf("tours : %d\n", nb_tours++);
        usleep(500000);
    }
}

void cart_SD_mode (void * arg) {
    while (1) {
        reset = 1;

        camera_usb_connection_1(NULL);

        while (usb_connected != 1) {
            usleep(500);
        }

        status = getModel(model, camera, &send_model);

        if (status != 0) {
            generateError(status);
            continue;
        }

        status = sd_card_lecture_mode (dossiers, dirs_n, camera, context);

        if (status < 0) continue;

        camera_usb_free_1(NULL);

        if (transfert_choice != 4) return;

        usleep(500000);
    }
}

void reconnexion_usb (void * arg) {
    while (1) {
        // printf("RECONNEXION USB ...\n");

        usleep(500000);
    }
}

void free_usb (void * arg) {
    while (1) {
        // Problème ici
        if (usb_freed == 0) {
            printf("LIBÉRATION USB ...\n");
            usb_freed = 1;
            gp_camera_exit(camera, context);
            gp_camera_free(camera);
        }
    }
}

void camera_usb_free_1(void * arg) {
    printf("LIBÉRATION USB ...\n");
    gp_camera_exit(camera, context);
    gp_camera_free(camera);
}

void camera_usb_connection (void * arg) {
    while (1) {
        if (transfert_choice > 0) {
            while (status != 0 || command_usb_reconnexion == 1) {
                printf("CONNEXION USB ...\n");
                status = gp_camera_new (&camera);
                handleError(status);
                status = gp_camera_init(camera, context);
                handleError(status);

                if (status < 0) {
                    printf("ERREUR DE CONNEXION !\n");
                    generateError(status);
                    status = gp_camera_exit(camera, context);
                    handleError(status);
                    status = gp_camera_free(camera);
                    handleError(status);
                    usleep(100000);
                    usb_connected = 0;
                }

                else {
                    if (connected_once > 0) {
                        usb_freed = 0;
                    }

                    // gp_camera_get_abilities (camera, &abilities);

                    connected_once++;
                    usb_connected = 1;
                    command_usb_reconnexion = 0;
                }
            }
        }

        usleep(500000);
    }
}

void camera_usb_connection_1 (void * arg) {
    if (transfert_choice > 0) {
        while (status != 0 || reset == 1) {
            printf("CONNEXION USB ...\n");
            printf("0");
            status = gp_camera_new (&camera);
            printf("1\n");
            handleError(status);
            status = gp_camera_init(camera, context);
            handleError(status);

            if (status < 0) {
                printf("ERREUR DE CONNEXION !\n");
                gp_camera_exit(camera, context);
                gp_camera_free(camera);
                usleep(500000); // Important sinon le programme consomme trop de CPU
                usb_connected = 0;
            }

            else {
                usb_connected = 1;
                reset = 0;
            }
        }
    }
}

// Problème ici lors du passage d'un mode à un autre

void script_apply_choice (void * arg) {
    int ret_task = 0;

    while (1) {
        printf("USB CONNECTED : %d\n", usb_connected);

        // Je pourrais mettre
        // if (allowChoiceChange == 1)

        switch (transfert_choice) {
            // Trouver un moyen de stoquer les historiques des tâches en TMP

            if (transfert_choice != prevTransfertChoice) {
                transfert_tasks[ttsk++] = transfert_choice;
                prevTransfertChoice = transfert_choice;
                printf("Changement de transfert.\n");
            }

            case 0 :
                printf("NONE\n");
            break;
            case 1 :
                printf("IMAGE AUTO\n");
                enable_transfert_image_auto(NULL);
            break;
            case 2 :
                printf("IMAGE SELECTIONNÉ\n");
                enable_transfert_image_selection(NULL);
            break;
            case 3 :
                printf("VIDEO AUTO\n");
                enable_transfert_video_auto(NULL);
            break;
            case 4 :
                printf("LECTURE CARTE SD\n");
                cart_SD_mode(NULL);
            break;
            default : printf("Erreur\n");
            break;
        }

        usleep(500000);
	}
}

void send_transferts_online (void * arg) {
    while (1) {
        send_medias_transfert_online(wifi_status);

        usleep(500000);
    }
}

// Travailler dessus car trop de répétitions ou rectifier côté réception
void manage_errors (void * arg) {
    while (1) {
        // printf("\nstatus : %d et prevStatus : %d\n\n", status, prevStatus);
        // if (status != prevStatus && error == 1 && status != 0) {
        //     printf("prevStatus : %d\n", prevStatus);
        //     prevStatus = status;
        //     res = send_status_request(status);
        //     printf("res : %d\n", res);
        //     printf("status : %d\n", status);
        //     handleError(status);
        //     error = 0;
        // }

        if (prevStatus != status) {
            printf("status : %d et prevStatus : %d\n", status, prevStatus);
            res = send_status_request(status);
            handleError(status);

            prevStatus = status;
        }

        usleep(50000);
    }
}

// Les valeurs de status oscillent

// Travailler dessus
void generateError (int status) {
    error = 1;
    // Peut être sauvegarder dans les logs
}

void sendModel (void * arg) {
    while (1) {
        if (send_model && wifi_status) {
            sendModelHTTP(model);
            send_model = 0;
        }
    }
}

void apply_networking (void * arg) {
    while (1) {
        if (wifi_status == 1) {
            rt_task_suspend(&task_send_files_online);
            rt_task_start(&task_save_files_offline, &send_medias, NULL);
        }

        else {
            rt_task_suspend(&task_save_files_offline);
            rt_task_start(&task_send_files_online, &save_medias, NULL);
        }

        usleep(100000);
    }
}
