#include "sd2.h"

int main (void) {
    Camera * camera;
    GPContext *context = sample_create_context();
    int status = 0;
    char *** dossiers = (char***) calloc(100, sizeof(char**));
    char ** dirs = (char**) calloc(10000, sizeof(char*));

    for (int i = 0; i < 100; i++) {
        dossiers[i] = (char**) calloc(10000, sizeof(char*));
        for (int j = 0; j < 10000; j++) {
            dossiers[i][j] = (char*) calloc(100, sizeof(char));
        }
    }

    for (int i = 0; i < 10000; i++) {
        dirs[i] = (char*) calloc(100, sizeof(char));
    }

    do {
        gp_camera_new (&camera);
        status = gp_camera_init(camera, context);
        
        if (status < 0) {
            handleError(status);
            gp_camera_exit(camera, context);
            gp_camera_free(camera);
            usleep(50000);
        }
    } while (status != 0);

    // unsigned int files_nb;

    // status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);
    // printf("Fichiers lus\n");

    // if (status < 0) printf("%d\n", status);

    // unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

    // status = get_sd_card_previews (files, nb_files, camera, context);


    printf("LECTURE DE LA LISTE ...\n");
    // status = get_folders (camera, context);

    status = recursive_directory(dossiers, dirs, camera, "/", context);
    printf("%d\n", status);
    handleError(status);

    printf ("LECTURE DE LA LISTE TERMINÉE.\n");

    if (status < 0) handleError(status);

    // unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);
    // printf("%u\n", nb_files);

    printf("AFFICHAGE DE LA LISTE ...\n");

    // afficher_carte_sd_liste(dossiers);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000 && dossiers[i][j][0] != 0; j++) {
            printf("%s\n", dossiers[i][j]);
        }
    }

    for (int i = 0; i < dir_nb; i++) {
        printf("%s\n", dirs[i]);
    }

    printf("AFFICHAGE DE LA LISTE TERMINÉ.\n");

    // status = get_sd_card_previews (files, nb_files, camera, context);
    // handleError(status);


    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000; j++) {
            free(dossiers[i][j]);
        }
        free(dossiers[i]);
    }

    for (int i = 0; i < 10000; i++) {
        free(dirs[i]);
    }

    free(dirs);
    free(dossiers);

    // FIN RÉPÉTITIONS

    return 0;
}




























static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

GPContext* sample_create_context() {
	GPContext *context;

	/* This is the mandatory part */
	context = gp_context_new();

	/* All the parts below are optional! */
        gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);

	/* also:
	gp_context_set_cancel_func    (p->context, ctx_cancel_func,  p);
        gp_context_set_message_func   (p->context, ctx_message_func, p);
        if (isatty (STDOUT_FILENO))
                gp_context_set_progress_funcs (p->context,
                        ctx_progress_start_func, ctx_progress_update_func,
                        ctx_progress_stop_func, p);
	 */
	return context;
}

/*int get_files_and_dirs (char *** dirs_b, Camera * camera, GPContext * context) {
    CameraList * folderList;
    CameraList * fileList;
    gp_list_new(&folderList);
    gp_list_new(&fileList);
    gp_list_reset(folderList);
    gp_list_reset(fileList);

    int i = 0, j, x = 0;
    
    int dir_nb;
    int file_nb;
    const char * dir;
    const char * file;
    char * tmp_dir = (char*) calloc(100, sizeof(char));
    char * folder = (char*) calloc(100, sizeof(char));
    char * chemin = (char*) calloc(100, sizeof(char));
    chemin[0] = '/';

    do {
        int status = gp_camera_folder_list_folders(camera, chemin, folderList, context);
        if (status < 0) return status;

        dir_nb = gp_list_count(folderList);
        sprintf(folder, "%s", );
        strcat(chemin, folder);
        for (i = 0; i < dir_nb; i++) {
            // chemin[0] = '/';
            printf("1 ...\n");
            status = gp_list_get_name(folderList, i, (const char**) &dir);
            if (status < 0) return status;

            printf("%s\n", dir);
            if (strcmp(chemin, "/") != 0) {
                sprintf(chemin, "/%s/%s", folder, dir);
            }

            else {
                sprintf(chemin, "%s/%s", folder, dir);
            }

            printf("chemin : %s\n", chemin);

            status = gp_list_reset(fileList);
            if (status < 0) return status;

            status = gp_camera_folder_list_files(camera, chemin, fileList, context);
            if (status < 0) return status;

            file_nb = gp_list_count(fileList);
            if (file_nb < 0) return status;

            for (unsigned int j = 0; j < file_nb; j++) {
                status = gp_list_get_name(fileList, j, (const char**) &file);
                // handleError(status);
                if (status < 0) return status;
                printf("4[i][j] ...\n%s\n", file);

                sprintf(dirs_b[x++][j], "%s/%s", chemin, file);
                printf("4[i][j] fini.\n");
            }
            printf("1 fini.\n");
        }
    } while (dir_nb != 0);

    free(folder);
    free(tmp_dir);
    free(chemin);
    gp_list_free(folderList);
    gp_list_free(fileList);

    return 0;
}*/
/*
char * get_files (Camera * camera, GPContext * context) {
    char text[100] = "/";
    static int x;
	const char *slash, *name;
	CameraList *list;
	int file_count, folder_count, r, len;
	char folder[MAX_FOLDER_LEN] = "/", basename[MAX_FILE_LEN] = "", *path = NULL;
    int state = 1;

	if (r < 0)
		return (NULL);
	len = strlen (basename);

	if (!state)
		x = 0;

	r = gp_list_new (&list);
	if (r < 0)
		return (NULL);
	r = gp_camera_folder_list_files (camera, folder, list, context);
	if (r < 0) {
		gp_list_free (list);
		return (NULL);
	}
	file_count = gp_list_count (list);
	if (file_count < 0) {
		gp_list_free (list);
		return (NULL);
	}
	if (x < file_count) {
		for (; x < file_count; x++) {
			r = gp_list_get_name (list, x, &name);
            printf("%s\n", name);
			if (r < 0)
				return (NULL);
			if (!strncmp (name, basename, len)) {
				x++;
				slash = strrchr (text, '/');
				if (!slash) {
					path = malloc (strlen (name) + 2);
					if (!path)
						return (NULL);
					strcpy (path, name);
					strcat (path, " ");
				} else {
					path = malloc (slash - text + 1 + strlen (name) + 2);
					if (!path)
						return (NULL);
					memset (path, 0, slash - text + 1 + strlen (name) + 2);
					strncpy (path, text, slash - text);
					strcat (path, "/");
					strcat (path, name);
					strcat (path, " ");
				}
				return (path);
			}
		}
	}

	r = gp_camera_folder_list_folders (camera, folder, list,
					   context);
	if (r < 0) {
		gp_list_free (list);
		return (NULL);
	}
	folder_count = gp_list_count (list);
	if (folder_count < 0) {
		gp_list_free (list);
		return (NULL);
	}
	if (x - file_count < folder_count) {
		for (; x - file_count < folder_count; x++) {
			r = gp_list_get_name (list, x - file_count, &name);
			if (r < 0) {
				gp_list_free (list);
				return (NULL);
			}
			if (!strncmp (name, basename, len)) {
				x++;
				slash = strrchr (text, '/');
				if (!slash) {
					path = malloc (strlen (name) + 2);
					if (!path)
						return (NULL);
					strcpy (path, name);
					strcat (path, "/");
				} else {
					path = malloc (slash - text + 1 + strlen (name) + 2);
					if (!path)
						return (NULL);
					memset (path, 0, slash - text + 1 + strlen (name) + 2);
					strncpy (path, text, slash - text);
					strcat (path, "/");
					strcat (path, name);
					strcat (path, "/");
				}
				gp_list_free (list);
				return (path);
			}
		}
		gp_list_free (list);
		return (NULL);
	}

	gp_list_free (list);
	return (NULL);
}
*/

/*
int get_dirs (char *** dirs_b, char * chemin, Camera * camera, GPContext * context) {
    CameraList * folderList;
    CameraList* fileList;
    gp_list_new(&folderList);
    gp_list_new(&fileList);

    int i = 0, x = 0, count, status;
    
    int dir_nb;
    const char * dir;
    char * folder = NULL;
    char * file = NULL;

    status = gp_camera_folder_list_folders(camera, chemin, folderList, context);
    handleError(status);
    if (status < 0) return status;

    status = gp_camera_folder_list_files(camera, chemin, fileList, context);
    handleError(status);
    if (status < 0) return status;

    count = gp_list_count(folderList);
    handleError(count);
    if (count < 0) return count;

    for (i = 0; i < count; i++) {
        printf("1\n");
        status = gp_list_get_name(folderList, i, (const char**) &dir);
        handleError(status);
        if (status < 0) return status;
        printf("1\n");

        printf("%s\n", dir);

        if (chemin[strlen(chemin)-1] != '/') {
            strcat(chemin, "/");
        }

        printf("1\n");
        
        strcat(chemin, dir);
        status = get_dirs (dirs_b, chemin, camera, context);
        printf("1\n");
        handleError(status);
    }

    return 0;
}
*/

static int
recursive_directory(char *** dossiers, char ** dirs, Camera *camera, const char *folder, GPContext *context) {
	int		i, ret, y;
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

		buf = malloc (strlen(folder) + 1 + strlen(newfolder) + 1);
		strcpy(buf, folder);
		if (strcmp(folder,"/"))		/* avoid double / */
			strcat(buf, "/");
		strcat(buf, newfolder);

		fprintf(stderr,"newfolder=%s\n", newfolder);
        strcpy(dirs[dir_nb++], newfolder);

		ret = recursive_directory (dossiers, dirs, camera, buf, context);
		free (buf);
		if (ret != GP_OK) {
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
    if (countFiles < 0) return countFiles;
    else if (countFiles > 0) x += 1;

    for (int i = 0; i < countFiles; i++) {
        gp_list_get_name (list, i, &newfile); /* only entry 0 needed */
        sprintf(dossiers[x][y++], "\n%s/%s", folder, newfile);
    }

	gp_list_free (list);
	return GP_OK;
}
/*
int get_folders (Camera * camera, GPContext * context) {
    CameraList * fileList;
    CameraList * folderList;
    gp_list_new(&fileList);
    gp_list_new(&folderList);
    char * chemin = (char*) calloc(100, sizeof(char));
    char * folder = (char*) calloc(100, sizeof(char));
    chemin[0] = '/';

    do {
        int status = gp_filesystem_list_folders(camera->fs, chemin, folderList, context);
        if (status < 0) return status;
        int i, j, count, file_count;
        char * dir;
        char * file;

        count = gp_list_count(list);
        printf("%d", count);

        for (i = 0; i < count; i++) {
            status = gp_list_get_name(folderList, i, (const char**) &dir);
            handleError(status);
            if (status < 0) return status;

            printf ("%s\n", dir);
            if (chemin[strlen(chemin) - 1] != '/') {
                sprintf(folder, "%s/%s", chemin, dir);
            }

            else {
                sprintf(folder, "%s%s", chemin, dir);
            }

            gp_list_reset(fileList);

            status = gp_filesystem_list_files(camera->fs, files, fileList, context);
            if (status < 0) return status;

            file_count = gp_list_count(fileList);
            if (file_count < 0) return file_count;

            for (j = 0; j < file_count; j++) {
                status = gp_list_get_name(fileList, i, (const char**) &file);
                handleError(status);
                if (status < 0) return status;
                printf("%s\n", file);
            }
        }
    } while (count != 0);

    free(chemin);
    gp_list_free(list);
    return 0;
}


int get_files_and_dirs (Fichiers ** dirs_b, Camera * camera, GPContext * context, char * chemin, int x, int nb_dir) {
    Fichiers * TEMP = new_fichiers_list();
    CameraList * folderList;
    CameraList * fileList;
    gp_list_new(&folderList);
    gp_list_new(&fileList);
    gp_list_reset(folderList);
    gp_list_reset(fileList);
    
    int dir_nb;
    int file_nb;
    const char * dir;
    const char * file;
    char * tmp_dir = (char*) calloc(100, sizeof(char));
    char * folder = (char*) calloc(100, sizeof(char));
    int status = gp_camera_folder_list_folders(camera, chemin, folderList, context);
    if (status < 0) return status;

    dir_nb = gp_list_count(folderList);

    if (dir_nb == 0) return 0;

    if (x < dir_nb || x == 0) {
        printf("1 ...\n");
        status = gp_list_get_name(folderList, 0, (const char**) &dir);
        if (status < 0) return status;
        printf("%s\n", dir);
        TEMP = insertenqueue_Fichiers(TEMP, dir, TYPE_DOSSIER, REL_FRERE);

        status = gp_list_reset(fileList);
        if (status < 0) return status;

        if (strcmp(chemin, "/") != 0) {
            sprintf(tmp_dir, "%s/%s", chemin, dir);
        }

        else {
            strcpy(tmp_dir, "/");
        }

        printf("%s\n", tmp_dir);
        status = gp_camera_folder_list_files(camera, tmp_dir, fileList, context);
        if (status < 0) return status;

        file_nb = gp_list_count(fileList);
        if (file_nb < 0) return status;

        TEMP = TEMP->fils;

        for (unsigned int j = 0; j < file_nb; j++) {
            status = gp_list_get_name(fileList, j, (const char**) &file);
            // handleError(status);
            if (status < 0) return status;
            printf("4[i][j] ...\n%s\n", TEMP->nom);

            TEMP = insertenqueue_Fichiers(TEMP, file, TYPE_JPG, REL_FILS);
            TEMP = TEMP->frere;
            printf("4[i][j] fini.\n");
        }

        if (strcmp(chemin, "/") != 0) {
            sprintf(folder, "%s/%s", chemin, dir);
            strcpy(chemin, folder);
        }

        else {
            sprintf(chemin, "/%s", dir);
        }

        printf("chemin : %s\n", chemin);

        // Appel récursif ici
        free(folder);
        free(tmp_dir);
        gp_list_free(folderList);
        gp_list_free(fileList);
        printf("1 fini.\n");
        *dirs_b = TEMP->frere;
        return get_files_and_dirs(dirs_b, camera, context, chemin, x+1, dir_nb); // + get_files_and_dirs(&TEMP->frere, camera, context, chemin, x+1, dir_nb);
    }

    return 0; // Pas d'erreur
}
*/

// int get_files_and_dirs (Dossiers ** dirs_b, Camera * camera, GPContext * context) {
//     Fichiers * TEMP = new_fichiers_list();
//     CameraList * folderList;
//     char * folder = (char*) calloc(100, sizeof(char));
//     gp_list_new(&folderList);
//     const char * dir;
//     int status = gp_camera_folder_list_folders(camera, "/", folderList, context);
//     if (status < 0) return status;

//     status = gp_list_get_name(folderList, 0, (const char**) &dir);
//     if (status < 0) return status;

//     printf("1 ...\n");
//     *dirs_b = insertenqueue_Dossiers(*dirs_b, dir, NULL, NULL);
//     printf("1 fini.\n");

//     // status = gp_list_reset(folderList);

//     char * tmp = (char*) calloc(100, sizeof(char));
//     char * tmp_dir = (char*) calloc(100, sizeof(char));

//     CameraList * fileList;
//     gp_list_new(&fileList);

//     unsigned int n = strlen(dir)-1;
//     strcpy(tmp_dir, "/store_");

//     unsigned int x = 0, z = 0;

//     while (dir[z++] != '_');
//     while (dir[z] != 0) {
//         tmp_dir[7+x++] = dir[z++];
//     }

//     // Insérer
//     printf("2 ...\n%s\n", tmp_dir);
//     TEMP = insertenqueue_Dossiers(TEMP, tmp_dir, NULL, NULL);
//     printf("2 fini.\n");

//     sprintf(folder, "%s/DCIM", tmp_dir);
//     // printf ("%s\n", folder);

//     // Peut etre ajouter une autre lecture des dossiers dans le répertoire

//     printf("3 ...\n");
//     // Insérer
//     TEMP = insertenqueue_Dossiers(TEMP, "DCIM", NULL, NULL);
//     printf("3 fini.\n");

//     status = gp_camera_folder_list_folders(camera,
// 		folder,
// 		folderList,
// 		context
// 	);
//     if (status < 0) return status;

//     int local_nb = gp_list_count(folderList);
//     int nb_files = 0;

//     if (local_nb < 0) return local_nb;

//     printf("4 ...\n");
//     for (unsigned int i = 0; i < local_nb; i++) {
//         // status = gp_list_reset(fileList);

//         const char * subdir;
//         status = gp_list_get_name(folderList, i, (const char**) &subdir);
//         // dossiers_fils->fils->fils = insertenqueue_Dossiers(dossiers_fils->fils->fils, subdir, NULL, NULL);
//         TEMP = insertenqueue_Fichiers(TEMP, subdir, TYPE_DOSSIER, REL_FRERE);

//         if (status < 0) return status;

//         // sprintf (tmp_dir, "%s/%s", folder, subdir);
//         // printf ("%s\n", tmp_dir);

//         // strcpy(dirs_n[i], tmp_dir);

//         status = gp_list_reset(fileList);
//         if (status < 0) return status;

//         status = gp_camera_folder_list_files(camera, tmp_dir, fileList, context);
//         if (status < 0) return status;

//         nb_files = gp_list_count(fileList);
//         if (nb_files < 0) return status;

//         for (unsigned int j = 0; j < nb_files; j++) {
//             const char * file;
//             status = gp_list_get_name(fileList, j, (const char**) &file);
//             // handleError(status);
//             if (status < 0) return status;
//             printf("4[i][j] ...\n");

//             // On ajoute les freres aux fils
//             TEMP = insertenqueue_Fichiers(TEMP->fils, file, TYPE_JPG, REL_FRERE);
//             printf("4[i][j] fini.\n");
//         }

//         TEMP = TEMP->frere; // Passage au frere suivant

//         // INSÉRER
//         printf("4[i] ...\n");
//         dossiers_fils->fils->fils = insertenqueue_Dossiers(dossiers_fils->fils->fils, subdir, fichiers_liste, NULL);
//         printf("4[i] fini.\n");

//         // sprintf(dirs_n[i], "/%s", dir);
//     }

//     printf("4 fini ...");

//     printf("Insérer le tout ...\n");

//     *dirs_b = insertenqueue_Dossiers(*dirs_b, "", NULL, dossiers_fils);
//     printf("Insertion totale terminée.\n");

//     free(tmp_dir);
//     free(folder);
//     free(tmp);
//     gp_list_free(folderList);
//     gp_list_free(fileList);
//     return 0; // Pas d'erreur
// }

// Dossiers* insertenqueue_Dossiers (Dossiers * liste, const char * valeur, Fichiers * fichiers, Dossiers * fils) {
//     Dossiers * P;
//     Dossiers * Q;
//     Dossiers * tete;

//     P = (Dossiers*) new_dossiers_list();
    
//     printf("%s\n", valeur);
//     if (valeur[0] != 0) strcpy(P->nom, valeur);
//     printf("2\n");
//     P->fichiers = fichiers;
//     // P->fils = fils;

//     if (fils != NULL) strcpy(P->fils->nom, fils->nom);
//     if (fils != NULL) P->fils->fichiers = fils->fichiers;

//     if (liste == NULL) {
//         return P;
//     }

//     else {
//         Q = liste;
//         tete = liste;
//         while (Q->suiv != NULL) {
//             Q = Q->suiv;
//         }
//         Q->suiv = P;
//         return tete;
//     }
// }

Fichiers * new_fichiers_list (void) {
    Fichiers * fichiers = (Fichiers*) malloc(sizeof(Fichiers));
    strcpy(fichiers->nom, "");
    fichiers->type = 0;
    fichiers->frere = (Fichiers*) malloc(sizeof(Fichiers));
    fichiers->fils = (Fichiers*) malloc(sizeof(Fichiers));

    return fichiers;
}

// Dossiers * new_dossiers_list (void) {
//     Dossiers * dossiers = (Dossiers*) malloc(sizeof(Dossiers)*100);
//     // dossiers = NULL;
//     dossiers->fichiers = (Fichiers*) new_fichiers_list();
//     // dossiers->fichiers = NULL;
//     // dossiers->nom[0] = 0;
//     dossiers->fils = (Dossiers*) malloc(sizeof(Dossiers)*100);
//     // dossiers->fils->nom[0] = 0;
//     dossiers->fils->fichiers = (Fichiers*) new_fichiers_list();
//     // dossiers->fils->fichiers = NULL;

//     return dossiers;
// }

void free_fichiers (Fichiers * fichiers) {
    free(fichiers);
}

// void free_dossiers (Dossiers * dossiers) {
//     // free(dossiers->nom);
//     free_fichiers(dossiers->fichiers);
//     // free(dossiers->fils->nom);
//     free_fichiers(dossiers->fils->fichiers);
//     free(dossiers);
// }

/*
int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    for (i = 0; i < nb; i++) {
        char * filename = (char*) getName(files[i], dir);
        printf("%s\n%s\n", dir, filename);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);

        status = gp_file_save(file, (const char*) targetPath);

        printf("SAUVEGARDE ...\n");

        if (status < 0) return status;
    }

    free(dir);
    free(targetPath);
    gp_file_free(file);
}

int sd_card_lecture_mode (Camera * camera, GPContext * context) {
    int status = 0;
    Dossiers * dossiers = (Dossiers*) new_dossiers_list();

    printf("LECTURE DE LA LISTE ...\n");
    status = get_files_and_dirs(&dossiers, camera, context);
    handleError(status);

    printf ("LECTURE DE LA LISTE TERMINÉE.\n");

    if (status < 0) return status;

    // unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);
    // printf("%u\n", nb_files);

    printf("AFFICHAGE DE LA LISTE ...\n");

    afficher_carte_sd_liste(dossiers);

    printf("AFFICHAGE DE LA LISTE TERMINÉ.\n");

    // status = get_sd_card_previews (files, nb_files, camera, context);
    // handleError(status);
    
    free_dossiers(dossiers);

    if (status < 0) return status;

    return 1;
}
*/

Fichiers* insertenqueue_Fichiers (Fichiers * liste, const char * valeur, int type, int rel) {
    Fichiers * P;
    Fichiers * Q;
    Fichiers * tete;

    P = (Fichiers*) malloc(sizeof(Fichiers));

    printf("%s\n", valeur);
    strcpy(liste->nom, valeur);
    liste->type = type;

    printf("1\n");
    if (liste == NULL) {
        return P;
    }

    else {
        Q = liste;
        tete = liste;
        if (rel == REL_FRERE) {
            printf("1\n");
            while (Q->frere != NULL) {
                Q = Q->frere;
            }
            printf("1\n");
            Q->frere = P;
            return tete;
        }

        else if (rel == REL_FILS) {
            printf("1\n");
            while (Q->fils != NULL) {
                Q = Q->fils;
            }
            printf("1\n");
            Q->fils = P;
            return tete;
        }
    }
}

void handleError(int status) {
    printf ("%s\n", gp_result_as_string(status));
}

// void afficher_fichiers_liste (Fichiers * fichiers) {
//     if (fichiers != NULL) {
//         printf("%s\n", fichiers->nom);
//         afficher_fichiers_liste(fichiers->suiv);
//     }
// }

// void afficher_carte_sd_liste (Dossiers * dossiers) {
//     if (dossiers != NULL) {
//         printf("%s\n", dossiers->nom);
//         afficher_fichiers_liste(dossiers->fichiers);
//         afficher_carte_sd_liste(dossiers->suiv);
//     }
// }

// status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);