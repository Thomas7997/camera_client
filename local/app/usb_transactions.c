#include "usb_transactions.h"

int x_sd = -1;

int reset_usb_dev (const char * filename) {
    int fd, rc;
    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("Error opening output file");
        return 1;
    }
    printf("Resetting USB device %s\n", filename);
    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
        perror("Error in ioctl");
        return 1;
    }
    close(fd);
}

static int
selection_wait_event (Camera * camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename) {
    int waittime = 100;
    static int nrofqueue=0;
    static int nrdownloads=0;

    static const char *bufferx;
    static const char *buffer;
	CameraEventType	evtype;
	CameraFilePath	*path;
	void		*data;
	int		retval;
    struct timeval	start, curtime;
    unsigned int x = 0;
    int selectionInterrupted = 0;

    int status = 0;
    int fd, rc;

    gettimeofday (&start, NULL);
	data = NULL;
	if (nrofqueue) waittime = 10; /* just drain the event queue */

	while (!selectionInterrupted) {
		int timediff;

		retval = gp_camera_wait_for_event(camera, 10, &evtype, &data, context);
		if (retval != GP_OK) {
			fprintf (stderr, "return from waitevent in trigger sample with %d\n", retval);
			return retval;
		}
		path = (CameraFilePath*) data;

        selectionInterrupted = evtype == GP_EVENT_CAPTURE_COMPLETE;

        if (evtype == GP_EVENT_CAPTURE_COMPLETE) {
            printf("Capture complete !\n");
            status = gp_camera_exit(camera, context);
            handleError(status);
            if (status < 0) return status;
            printf ("1\n");

            reset_usb_dev(filename);

            status = gp_camera_init(camera, context);
            if (status < 0) return status;
            printf ("4\n");
        }

        if (evtype == GP_EVENT_FILE_CHANGED && path) {
            printf("%s/%s changed !\n", path->folder, path->name);
            sprintf(files[x], "%s/%s", path->folder, path->name);
            printf("Transferring %s from path %s\n", path->name, files[x]);
            transferer_nom_auto(files[x++], context, camera);
        }
    }

    *nroftransferts = x;
	return GP_OK+x;
}  

int control_selection (Camera * camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename) {
    // Will be the new main process of a task
    FILE * STATE;
    int status = 0;
    int st = 0;
    int x = 0;

    while (1) {
        STATE = fopen("../data/tmp/selection.txt", "r");
        if (fscanf(STATE, "%d", &st) != 1) continue;

        if (st) {
            printf ("Working ...\n");
            printf ("usb path : %s\n", filename);
            status = selection_wait_event(camera, context, nroftransferts, files, filename);
            fclose(STATE);
            if (status < 0) {
                // Go to usb connection
                // Send alerts

                handleError(status);
            }

            STATE = fopen("../data/tmp/selection.txt", "w");
            fprintf(STATE, "0");
            fclose (STATE);
        }

        else {
		    printf("Interrupted\n");
		    fclose(STATE);
		    usleep(500000);
        }
	}

    *nroftransferts = x;
	return GP_OK;
}


// Pour l'envoi
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera) {
    int i = 0;

    char * commande = (char*) calloc(250, sizeof(char));
    int file_transfered = 0, first_use = -1;

    FILE * HISTORIQUE;
    FILE * RAW_FORMAT;
    FILE * NORMAL_FORMAT;
    FILE * FIRST_USE;

    // Il faudrait aussi vérifier si les fichiers sont bien ouverts

    do {
        HISTORIQUE = fopen("../data/images/historique.txt", "a+");

        if (HISTORIQUE == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
            printf("Error %d \n", errno);
            fclose(HISTORIQUE);
        }
    } while (HISTORIQUE == NULL);


    do {
        FIRST_USE = fopen("../data/tmp/first_use.txt", "r");

        if (FIRST_USE == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
            printf("Error %d \n", errno);
            fclose(FIRST_USE);
        }
    } while (FIRST_USE == NULL);


    do {
        NORMAL_FORMAT = fopen("../data/tmp/normal_format.txt", "r");

        if (NORMAL_FORMAT == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
            printf("Error %d \n", errno);
            fclose(NORMAL_FORMAT);
        }
    } while (NORMAL_FORMAT == NULL);

    do {
        RAW_FORMAT = fopen("../data/tmp/raw_format.txt", "r");

        if (RAW_FORMAT == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
            printf("Error %d \n", errno);
            fclose(RAW_FORMAT);
        }
    } while (RAW_FORMAT == NULL);

    unsigned int raw, normal;

    fscanf(RAW_FORMAT, "%u", &raw);
    fscanf(NORMAL_FORMAT, "%u", &normal);

    if (raw == 0 && normal == 0) return 0; // Car aucun fichier transféré

    char ** hist_lines = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    int x;

    for (x = 0; x < MAX_CAPTURES; x++) {
        hist_lines[x] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    x = 0;

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUE));

    char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    int status = 0;
    CameraFile * file;
    gp_file_new(&file);
    char * filename = (char*) calloc(100, sizeof(char));

    if (fscanf(FIRST_USE, "%d", &first_use) != 1)
    // Erreur
    printf("Erreur de lecture du fichier.\n");

    for (i = 0; i < n_transferts; i++) {
        strcpy(dossier, "");
        filename = getName(liste[i], dossier);
        // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

        // A corriger
        if (first_use == 1) {
            // ECRIRE TOUS LES FICHIERS 5 ÉTOILES DANS L'HISTORIQUE
            fprintf(HISTORIQUE, "%s\n", filename);
            continue;
        }

        file_transfered = compare_file_historique(filename, hist_lines, x);
        if (file_transfered == 0) {
            if (normal == 1) {
                sprintf(commande, "../data/images/gets/%s", filename);
                printf ("%s\n", commande);

                status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);

                handleError(status);
                printf("%s\n", dossier);

                if (status < 0) return status;

                // TÉLÉCHARGEMENT DE LA PHOTO UNIQUEMENT
                status = gp_file_save(file, (const char*) commande);

                if (status < 0) return status;
            }

            if (raw == 1) {
                status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_RAW, file, context);

                sprintf(commande, "../data/images/gets/RAW_%s", filename);
                printf ("%s\n", commande);

                handleError(status);
                printf("%s\n", dossier);

                if (status < 0) return status;

                // TÉLÉCHARGEMENT DE LA PHOTO UNIQUEMENT
                status = gp_file_save(file, (const char*) commande);

                if (status < 0) return status;
            }

            // Envoyer le nom du nouveau fichier transféré au socket

            // ÉCIRE DANS L'HISTORIQUE DES TRANSFERTS
            printf("%s\n", commande);
            printf ("Transfert !\n");
            fprintf(HISTORIQUE, "%s\n", filename);
        }
    }

    if (first_use == 1) {
        FILE * FIRST_USE_W;

        do {
            FIRST_USE_W = fopen("../data/tmp/first_use.txt", "w");

            if (FIRST_USE_W == NULL) {
                printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
                printf("Error %d \n", errno);
                fclose(FIRST_USE_W);
            }
        } while (FIRST_USE_W == NULL);

        printf ("ECRITURE DE LA PREMIÈRE UTILISATION.\n");
        fprintf (FIRST_USE_W, "0");
        fclose(FIRST_USE_W);
    }

    for (x = 0; x < MAX_CAPTURES; x++) {
        free(hist_lines[x]);
    }

    free(current_file);
    fclose(HISTORIQUE);
    free(hist_lines);
    free(filename);
    free(commande);
    free(dossier);
    gp_file_free(file);
    fclose(RAW_FORMAT);
    fclose(NORMAL_FORMAT);
    fclose(FIRST_USE);

    return x;
}

// L'idéal serait de directement transferer un nom individuelement
int transferer_nom_auto (char * nom, GPContext * context, Camera * camera) {
    int i = 0;

    char * commande = (char*) calloc(250, sizeof(char));
    int file_transfered = 0;

    char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    int status = 0;
    CameraFile * file;
    gp_file_new(&file);
    char * filename = (char*) calloc(100, sizeof(char));

    strcpy(dossier, "");
    filename = getName(nom, dossier);
    // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

    // A corriger
    sprintf(commande, "../data/images/gets/%s", filename);
    printf ("%s\n", commande);
    status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);

    if (status < 0) return status;

    // TÉLÉCHARGEMENT DE LA PHOTO UNIQUEMENT
    status = gp_file_save(file, (const char*) commande);
    handleError(status);

    if (status < 0) return status;

    // Envoyer le nom du nouveau fichier transféré au socket

    // ÉCRIRE DANS L'HISTORIQUE DES TRANSFERTS
    printf ("%s\n", commande);
    printf ("Transfert !\n");

    free(current_file);
    free(filename);
    free(commande);
    free(dossier);
    gp_file_free(file);

    return 0;
}

// Pour l'envoi
int transferer_noms_auto (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera) {
    int i = 0;

    char * commande = (char*) calloc(250, sizeof(char));
    int file_transfered = 0;

    FILE * HISTORIQUE;

    do {
        HISTORIQUE = fopen("/home/remote/camera_client/local/data/images/historique.txt", "a+");
        printf("Error %d \n", errno);
        if (HISTORIQUE == NULL) printf("LECTURE DE FICHIER HISTORIQUE.\n");
    } while (HISTORIQUE == NULL);

    char ** hist_lines = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    int x;

    for (x = 0; x < MAX_CAPTURES; x++) {
        hist_lines[x] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    x = 0;

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUE));

    char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    int status = 0;
    CameraFile * file;
    gp_file_new(&file);
    char * filename = (char*) calloc(100, sizeof(char));

    for (i = 0; i < n_transferts; i++) {
        strcpy(dossier, "");
        filename = getName(liste[i], dossier);
        // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

        // A corriger
        sprintf(commande, "../data/images/gets/%s", filename);
        printf ("%s\n", commande);
        status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);
        handleError(status);
        printf("%s\n", dossier);

        if (status < 0) return status;

        // TÉLÉCHARGEMENT DE LA PHOTO UNIQUEMENT
        status = gp_file_save(file, (const char*) commande);

        if (status < 0) return status;

        // Envoyer le nom du nouveau fichier transféré au socket

        // ÉCRIRE DANS L'HISTORIQUE DES TRANSFERTS
        printf("%s\n", commande);
        printf ("Transfert !\n");
        fprintf(HISTORIQUE, "%s\n", filename);
    }

    for (x = 0; x < MAX_CAPTURES; x++) {
        free(hist_lines[x]);
    }

    free(current_file);
    fclose(HISTORIQUE);
    free(hist_lines);
    free(filename);
    free(commande);
    free(dossier);
    gp_file_free(file);

    return 0;
}

int compare_file_historique (char * file, char ** historique, int lines) {
    int i;
    unsigned int file_s = strlen(file)-1;

    for (i = 0; i < lines; i++) {
        if (strncmp(historique[i], file, file_s) == 0) {
           return 1;
        }
    }

    return 0;
}

// Fonction principale pour la lecture de fichiers
int get_files (char ** files, Camera * camera, GPContext * context, unsigned int * x) {
    x_sd = -1;
    int status = recursive_directory(files, camera, "/", context, x);
    handleError(status);

    if (status < GP_OK) return status;

    return GP_OK;
}

// Bonne version
int
recursive_directory(char ** files, Camera *camera, const char *folder, GPContext *context, unsigned int * x) {
	int		i, ret;
	CameraList	*list;
	const char	*newfile;
	CameraFileInfo	fileinfo;
	CameraFile	*file;

	ret = gp_list_new (&list);
	if (ret < GP_OK) {
		printf ("Could not allocate list.\n");
		return ret;
	}

	ret = gp_camera_folder_list_folders (camera, folder, list, context);
	if (ret < GP_OK) {
		printf ("Could not list folders.\n");
		
        gp_list_free (list);
		return ret;
	}
	gp_list_sort (list);

	for (i = 0; i < gp_list_count (list); i++) {
		const char *newfolder;
		char *buf;
		int havefile = 0;

		gp_list_get_name (list, i, &newfolder);

		if (!strlen(newfolder)) continue;

		buf = (char*) malloc (strlen(folder) + 1 + strlen(newfolder) + 1);
		strcpy(buf, folder);
		if (strcmp(folder,"/"))		/* avoid double / */
			strcat(buf, "/");
		strcat(buf, newfolder);

		fprintf(stderr,"newfolder=%s\n", newfolder);

		ret = recursive_directory (files, camera, buf, context, x);
		free (buf);
		if (ret < GP_OK) {
			gp_list_free (list);
			printf ("Failed to recursively list folders.\n");
			return ret;
		}
		if (havefile) /* only look for the first directory with a file */
			break;
	}
	gp_list_reset (list);

	ret = gp_camera_folder_list_files (camera, folder, list, context);
	if (ret < GP_OK) {
		gp_list_free (list);
		printf ("Could not list files.\n");
		return ret;
	}
	gp_list_sort (list);
	if (gp_list_count (list) <= 0) {
		gp_list_free (list);
		return GP_OK;
	}

    int countFiles = gp_list_count(list);
    // printf("%d\n", countFiles);
    if (countFiles < 0) return countFiles;

    for (int z = 0; z < countFiles; z++) {
        gp_list_get_name (list, z, &newfile);
        sprintf(files[++x_sd], "%s/%s", folder, newfile);
    }

    // Reset here
    *x = x_sd + 1;

	gp_list_free (list);
	return GP_OK;
}

// Need to test

int download_file (char ** files, char * name, Camera * camera, GPContext * context, char * err_name) {
    char * folder = (char*) calloc(100, sizeof(char));
    int status;

    // Find path
    printf("GETTING FOLDER ...\n");

    int index = find_dir_filename (files, name, folder);

    if (index < 0) {
        return -(index);
    }

    printf("GOT FOLDER !\n");

    // Download
    printf("GETTING FILE ...\n");
    CameraFile * file;
    status = gp_file_new(&file);

    if (status < 0) {
        return status;
    }

    status = gp_filesystem_get_file (camera->fs, folder, name, GP_FILE_TYPE_NORMAL, file, context);

    if (status < 0) {
        strcpy(err_name, name);
        return status;
    }

    printf("GOT FILE !\n");
    printf("SAVING FILE ...\n");

    char * path = (char*) calloc(100, sizeof(char));
    sprintf(path, "../data/images/downloads/%s", name);

    status = gp_file_save (file, (const char*) path);

    if (status < 0) {
        strcpy(err_name, name);
        return status;
    }

    printf("FILE SAVED !\n");
    gp_file_free(file);
    free(path);

    return GP_OK;
}

int delete_file (char ** files, char * name, Camera * camera, GPContext * context, char * err_name) {
    char * folder = (char*) calloc(100, sizeof(char));
    int status;

    // Find path
    printf("GETTING FOLDER ...\n");

    int index = find_dir_filename (files, name, folder);

    if (index < 0) return -(index);

    printf("GOT FOLDER !\n");

    // Delete
    status = gp_filesystem_delete_file (camera->fs, folder, name, context);

    if (status < 0) {
        strcpy(err_name, name);
        return status;
    }

    return GP_OK;
}

int lsusb_find_camera (char * p) {
    // List usb devices
    // Find camera in the list

    system("lsusb > ../data/tmp/lsusb.txt");
    FILE * LSUSB = fopen("../data/tmp/lsusb.txt", "r");
    char ** lines = (char**) calloc(30, sizeof(char*));
    int x = 0;

    for (int i = 0; i < 30; i++) {
        lines[i] = (char*) calloc(100, sizeof(char));
    }

    while (fgets(lines[x++], 99, LSUSB));

    int size = x;
    char * nmbrStr = (char*) calloc(5, sizeof(char));

    for (x = 0; x < size-1; x++) {
        // Ports 1, 2, 3 or reserved
        int y = 0;
        strcpy(nmbrStr, "");
        while (y<15) y++;
        // while (lines[x][y++] != ' ');
        for (int c = 0; c < 3; c++) nmbrStr[c] = lines[x][y+c];

        if (strcmp(nmbrStr, "001") && strcmp(nmbrStr, "002") && strcmp(nmbrStr, "003")) {
            sprintf(p, "/dev/bus/usb/001/%s", nmbrStr);
            return 0;
        }
    }

    free(nmbrStr);

    for (int i = 0; i < 30; i++) {
        free(lines[i]);
    }

    fclose (LSUSB);
    free(lines);

    return -1;
}
