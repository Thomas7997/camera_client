#include "telecam.h"

int main (void) {
	// Initialisations
    dossiers = (char***) calloc(MIN_DIRS, sizeof(char**)); // A supprimer plus tard
    files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    images_list = (char**) calloc(PART_NB, sizeof(char*));
    dir_sizes = (unsigned int*) calloc(MIN_DIRS, sizeof(unsigned int*));
    liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    transferts_tmp = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    model = (char*) calloc(MAX_CAPTURES, sizeof(char));
    photos = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    add = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    supp = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    cld_files = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    downloads = (char**) calloc(MAX_DOWNLOADS, sizeof(char*));
    deletes = (char**) calloc(MAX_DOWNLOADS, sizeof(char*));
    downloaded_files = (char**) calloc(MAX_DOWNLOADS, sizeof(char*));
    deleted_files = (char**) calloc(MAX_DOWNLOADS, sizeof(char*));
    prevModel = (char*) calloc(TAILLE_NOM, sizeof(char));

    // BOOLS
    all_deleted = 1;
    all_downloaded = 1;

    // For error handling
    prevStatus = status = 0;
    prevCamera_status = camera_status = 1;
    operation_status = prevOperation_status = OP_NO_STATUS;
    send_op_status = 0;
    operation_mode = OP_NONE;
    operation_name = (char*) calloc(TAILLE_NOM, sizeof(char));

    for (unsigned int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_DIR_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_DIR_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        images_list[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_DOWNLOADS; i++) {
        downloads[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        deletes[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        downloaded_files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        deleted_files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }
    
    transferts_send = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        transferts_send[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts_tmp[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        photos[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        add[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        supp[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
        cld_files[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    context = sample_create_context();

	// Main tasks

    // SD
    printf("SD SCRIPTS\n");

    // Scripts de vérification
	// result = rt_task_spawn (&task_transfert_choice, "TRANSFERT CHOICE", 4096, 99, TASK_PERM, &check_transfert_choice, NULL);
	// // result = rt_task_spawn (&task_wifi, "WIFI_STATUS", 4096, 99, TASK_PERM, &check_wifi_status, NULL);
    // // result = rt_task_spawn (&task_manage_errors, "MANAGE ERRORS", 4096, 99, TASK_PERM, &manage_errors, NULL);

	// // Scripts d'action
    // printf("ACTIONS SCRIPTS\n");
	// result = rt_task_create (&task_save_files_offline, "SAVE MEDIAS", 4096, 99, TASK_PERM);
	// result = rt_task_create (&task_send_files_online, "SEND MEDIAS", 4096, 99, TASK_PERM);
    // result = rt_task_create (&task_enable_transfert, "LANCER TRANSFERT IMAGE AUTO", 10000, 99, TASK_PERM);

    // // MAIN SCRIPTS
    // printf("MAIN SCRIPTS\n");
    // result = rt_task_spawn (&task_apply_choice, "APPLIQUER LE CHOIX DE L'UTILISATEUR", 4096, 99, TASK_PERM, &script_apply_choice, NULL);
    // result = rt_task_spawn (&task_wifi_transfert, "APPLIQUER LE TRANSFERT WIFI AUTONOME", 4096, 99, TASK_PERM, &send_transferts_online, NULL);

    // // NOTIFICATION
    // printf("NOTIFICATION SCRIPTS\n");
    // result = rt_task_spawn(&task_send_model, "SEND CAMERA MODEL", 4096, 99, TASK_PERM, &send_model_fn, NULL);
    // result = rt_task_spawn(&task_notify_camera_status, "SEND CAMERA STATUS", 4096, 99, TASK_PERM, &checkAndNotifyCameraStatus, NULL);

    // Thread creations
    pthread_create(&id_transfert_choice, NULL, check_transfert_choice, NULL);
    pthread_create(&id_wifi, NULL, check_wifi_status, NULL);
    pthread_create(&id_manage_errors, NULL, manage_errors, NULL);
    pthread_create(&id_apply_choice, NULL, script_apply_choice, NULL);
    pthread_create(&id_wifi_transfert, NULL, send_transferts_online, NULL);
    pthread_create(&id_notify_camera_status, NULL, checkAndNotifyCameraStatus, NULL);

    // Thread calls
    pthread_join(id_transfert_choice, NULL);
    pthread_join(id_wifi, NULL);
    pthread_join(id_manage_errors, NULL);
    pthread_join(id_apply_choice, NULL);
    pthread_join(id_wifi_transfert, NULL);
    pthread_join(id_notify_camera_status, NULL);

    while (1) {
		pause();
	}

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        free(transferts_send[e]);
        free(transferts_tmp[e]);
        free(photos[e]);
    }

    free(transferts_send);
    free(operation_name);

    // free_usb(NULL);
    
	// rt_task_delete(&task_transfert_choice);
	// rt_task_delete(&task_wifi);
	// rt_task_delete(&task_save_files_offline);
	// rt_task_delete(&task_send_files_online);
    // rt_task_delete(&task_apply_choice);
    // rt_task_delete(&task_wifi_transfert);
    // rt_task_delete(&task_manage_errors);

    for (int d = 0; d < MIN_DIRS; d++) {
        for (int dy = 0; dy < MAX_DIR_CAPTURES; dy++) {
            free(dossiers[d][dy]);
            dossiers[d][dy] = NULL;
        }
        free(dossiers[d]);
        dossiers[d] = NULL;
    }

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(files[i]);
        files[i] = NULL;
        free(liste_captures[i]);
        liste_captures[i] =  NULL;
        free(add[i]);
        add[i] = NULL;
        free(supp[i]);
        supp[i] = NULL;
        free(cld_files[i]);
        cld_files[i] = NULL;
    }

    for (unsigned int i = 0; i < MAX_DOWNLOADS; i++) {
        free(downloads[i]);
        downloads[i] = NULL;
        free(deletes[i]);
        deletes[i] = NULL;
        free(downloaded_files[i]);
        downloaded_files[i] = NULL;
        free(deleted_files[i]);
        deleted_files[i] = NULL;
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        free(images_list[i]);
        images_list[i] = NULL;
    }

    free(deletes);
    deletes = NULL;
    free(dir_sizes);
    dir_sizes = NULL;
    free(dossiers);
    dossiers = NULL;
    free(files);
    files = NULL;
    free(images_list);
    images_list = NULL;
    free(liste_captures);
    liste_captures = NULL;
    free(transferts_tmp);
    transferts_tmp = NULL;
    free(model);
    model = NULL;
    free(photos);
    photos = NULL;
    free(prevModel);
    prevModel = NULL;
}

void * check_transfert_choice (void * arg) {
	while (1) {
        FILE * TRANSFERT_CHOICE; // Changera de chemin

        do {
            // printf("READING TRANSFERT CHOICE ...\n");
            TRANSFERT_CHOICE = fopen("../data/tmp/transfert_choice.txt", "r");
            if (TRANSFERT_CHOICE == NULL) {
                printf("Erreur de lecture du fichier TRANSFERT CHOICE.\n");
                
                fclose(TRANSFERT_CHOICE);
            }
        } while (TRANSFERT_CHOICE == NULL);

        fscanf(TRANSFERT_CHOICE, "%d", &transfert_choice);
        // prevTransfertChoice = transfert_choice;
		// printf ("CHECK TRANSFERT CHOICE\n");
        fclose(TRANSFERT_CHOICE);
        usleep(5000);
	}
}

void * check_wifi_status (void * arg) {
	while (1) {
        FILE * WIFI;

        do {
            WIFI = fopen("../data/tmp/wifi_status.txt", "r");
            if (WIFI == NULL) {
                printf("Erreur de lecture du fichier.\n");
                
                fclose(WIFI);
            }
        } while (WIFI == NULL);

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

void enable_transfert_on_metadata_change (void * arg) {
    while (1) {
        printf("TRANSFERT D'IMAGES SÉLECTIONNÉ LANCÉ\n");

        reset = 1;

	printf ("STARTING USB CAMERA CONNECTION ...\n");

        camera_usb_connection_1 (NULL);

        // Add metadata change selection
        printf ("ENDED USB CAMERA CONNECTION.\n");
	char * filename = (char*) calloc(100, sizeof(char));

        int ret = lsusb_find_camera (filename);
        if (ret != 0) {
            continue;
        }

        unsigned int nroftransferts;

        status = control_selection (camera, context, &nroftransferts, transferts_send, filename);

	camera_usb_free_1(NULL);

        if (status < 0) {
            camera_status = status;
	    // continue;
        }

        if (!transfert_choice) return;
    }
}

void reconnexion_usb (void * arg) {
    while (1) {
        printf("RECONNEXION USB ...\n");

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
            status = gp_camera_new (&camera);
            handleError(status);
            status = gp_camera_init(camera, context);
            handleError(status);

            if (status < 0) {
                printf("ERREUR DE CONNEXION !\n");
                camera_status = status;
                status = gp_camera_exit(camera, context);
                handleError(status);
                status = gp_camera_free(camera);
                handleError(status);
                usb_connected = 0;
                usleep(500000); // because too much CPU
            }

            else {
                usb_connected = 1;
                reset = 0;

                status = getModel(model, camera, &send_model);

                if (status != 0) {
                    generateError(status);
                    continue;
                }

                printf("%s\n", model);

                camera_status = 0;
            }
        }
    }
}

// Problème ici lors du passage d'un mode à un autre

void* script_apply_choice (void * arg) {
    int ret_task = 0;

    while (1) {
        printf("USB CONNECTED : %d\n", usb_connected);

        switch (transfert_choice) {
            if (transfert_choice != prevTransfertChoice) {
                // transfert_tasks[ttsk++] = transfert_choice;
                prevTransfertChoice = transfert_choice;
                printf("Changement de transfert.\n");
            }

            case 0 :
                printf("NONE\n");
            break;
            case 1 :
                printf("ON METADATA CHANGE\n");
                enable_transfert_on_metadata_change(NULL);
            break;
            default : printf("Erreur\n");
            break;
        }

        usleep(5000);
	}
}

void * send_transferts_online (void * arg) {
    while (1) {
        send_medias_transfert_online(wifi_status);

        usleep(500000);
    }
}

// Travailler dessus car trop de répétitions ou rectifier côté réception
void * manage_errors (void * arg) {
    while (1) {
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

void * end_model_fn (void * arg) {
    while (1) {
        if (model[0] && wifi_status && strcmp(prevModel, model)) {
            int st = sendModelHTTP(model);
            // if (st == 0) clearStr(model);
            strcpy(prevModel, model);
        }

        usleep(100000);
    }
}

void write_choice (int choice) {
    FILE * W = fopen("../data/tmp/transfert_choice.txt", "w");

    fprintf(W, "%d", choice);

    fclose(W);
}

void * checkAndNotifyCameraStatus (void * arg) {
    int res = 0;
    
    while (1) {
        if (wifi_status && camera_status != prevCamera_status) {
            prevCamera_status = camera_status;

            do {
                res = send_camera_status(camera_status);
                
                if (res != CURLE_OK) {
                    printf("NO WIFI !\n");
                    sleep(3);
                }
            } while (res != CURLE_OK);
        }

        usleep(10000);
    }
}
