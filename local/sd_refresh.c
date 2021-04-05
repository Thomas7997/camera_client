#include "sd_refresh.h"
// Fonction diff_sd_list_refresh 

int x_sd = -1, dir_nb_sd = 0;

int main (void) {
    Camera * camera;
    GPContext *context = sample_create_context();
    int status = 0, nb_directories;
    char *** dossiers = (char***) calloc(10, sizeof(char**));
    char ** dirs = (char**) calloc(10, sizeof(char*));
    char ** supp = (char**) calloc(10000, sizeof(char*));
    char ** add = (char**) calloc(10000, sizeof(char*));
    char ** files = (char**) calloc(10000, sizeof(char*));
    char ** cld = (char**) calloc(10000, sizeof(char*));

    for (int i = 0; i < 10000; i++) {
        add[i] = (char*) calloc(10000, sizeof(char));
        supp[i] = (char*) calloc(10000, sizeof(char));
    }

    for (int i = 0; i < 10; i++) {
        dossiers[i] = (char**) calloc(10000, sizeof(char*));
        for (int j = 0; j < 10000; j++) {
            dossiers[i][j] = (char*) calloc(100, sizeof(char));
        }
    }

    for (int i = 0; i < 10; i++) {
        dirs[i] = (char*) calloc(100, sizeof(char));
    }

    for (int i = 0; i < 10000; i++) {
        files[i] = (char*) calloc(100, sizeof(char));
        cld[i] = (char*) calloc(100, sizeof(char));
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

    printf("LECTURE DE LA LISTE ...\n");

    status = recursive_directory(dossiers, dirs, camera, "/", context);
    printf("%d\n", status);

    if (status < 0) handleError(status);

    printf ("LECTURE DE LA LISTE TERMINÉE.\n");

    unsigned int n_files = dossiers_to_list (dossiers, files, dir_nb_sd), n_add, n_supp, n_cld;

    printf("AFFICHAGE DE LA LISTE ...\n");

    for (int i = 0; i < n_files; i++) {
        printf("%s\n", files[i]);
    }

    // for (int i = 0; i < dir_nb_sd; i++) {
    //     printf("%s\n", dirs[i]);
    // }

    printf("AFFICHAGE DE LA LISTE TERMINÉ.\n");

    // Lire le dossier cloud
    n_cld = read_cld(cld);

    // Comparaison des différences
    diff_sd_list_refresh(supp, add, cld, files, n_cld, n_files, &n_add, &n_supp);

    // status = get_sd_card_previews (dossiers, x_sd, camera, context);
    // handleError(status);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10000; j++) {
            free(dossiers[i][j]);
        }
        free(dossiers[i]);
    }

    for (int i = 0; i < 10; i++) {
        free(dirs[i]);
    }

    for (int i = 0; i < 10000; i++) {
        free(supp[i]);
        free(add[i]);
    }

    for (int i = 0; i < 10000; i++) {
        free(files[i]);
        free(cld[i]);
    }

    free(supp);
    free(add);
    free(dirs);
    free(dossiers);
    free(files);
    free(cld);

    // FIN RÉPÉTITIONS

    return 0;
}

int get_sd_card_previews (char *** dossiers, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(100, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    printf("%d\n", nb);
    for (i = 0; i < nb; i++) {
        j = 0;
        while (dossiers[i][j][0] != 0) {
            printf("1\n");
            char * filename = (char*) getName(dossiers[i][j++], dir);
            printf("1\n");
            printf("%s\n%s\n", dir, filename);
            status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
            printf("RECEPTION...\n");

            if (status < 0) return status;

            // sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);
            sprintf(targetPath, "./data/images/cloud/%s", filename);
            status = gp_file_save(file, (const char*) targetPath);

            printf("SAUVEGARDE ...\n");

            if (status < 0) return status;
        }
    }

    free(dir);
    free(targetPath);
    gp_file_free(file);
    return 0;
}

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

		buf = (char*) malloc (strlen(folder) + 1 + strlen(newfolder) + 1);
		strcpy(buf, folder);
		if (strcmp(folder,"/"))		/* avoid double / */
			strcat(buf, "/");
		strcat(buf, newfolder);

		fprintf(stderr,"newfolder=%s\n", newfolder);
        printf("%d\n", dir_nb_sd);
        strcpy(dirs[dir_nb_sd++], newfolder);

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
    if (countFiles > 0) {
        x_sd += 1;
        y = 0;
    }

    for (int i = 0; i < countFiles; i++) {
        gp_list_get_name (list, i, &newfile); /* only entry 0 needed */
        sprintf(dossiers[x_sd][y], "%s/%s", folder, newfile);
        printf("%s\n", dossiers[x_sd][y++]);
    }

	gp_list_free (list);
	return GP_OK;
}


int sd_card_lecture_mode (char *** dossiers, char ** dirs_n, Camera * camera, GPContext * context) {
    int status = 0;

    int i, j, number = 0;
    unsigned int * dir_sizes = (unsigned int*) calloc(10, sizeof(int));
    unsigned int files_nb;

    for (unsigned int e = 0; e < 10; e++) {
        for (unsigned int j = 0; j < 10000; j++) {
            strcpy(dossiers[e][j], "");
        }
        strcpy(dirs_n[e], "");
    }

    x_sd = -1;
    dir_nb_sd = 0;

    status = recursive_directory(dossiers, dirs_n, camera, "/", context);
    handleError(status);

    status = get_sd_card_previews (dossiers, x_sd, camera, context);
    handleError(status);

    if (status < 0) return status;

    free(dir_sizes);

    return 1;
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

void handleError(int status) {
    printf ("%s\n", gp_result_as_string(status));
}

char * getName (char * buf, char * dossier) {
    unsigned int n = strlen(buf), x = 0, y = 0;
    char * buffer = (char*) calloc(100, sizeof(char));

    unsigned int i = n-1;

    while (buf[i] != '/') {
        buffer[x++] = buf[i--];
    }

    while (i > 0) {
        dossier[y++] = buf[i-1];
        i--;
    }

    mirroir(buffer, x);
    mirroir(dossier, y);

    return buffer;
}

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

int max (int a, int b) {
    if (a > b) return a;
    return b;
}

void diff_sd_list_refresh (char ** supp, char ** add, char ** cld_files, char ** sd_files, unsigned int cld_size, unsigned int sd_size, unsigned int * n_supp, unsigned int * n_add) {
    *n_add = 0;
    *n_supp = 0;

    unsigned int i = 0, y = 0, newFile, x = 0, a = 0;
    char * dir = (char*) calloc(100, sizeof(char));
    char * suppName = (char*) calloc(100, sizeof(char));

    for (i = 0; i < sd_size; i++) {
        newFile = 0;
        char * name = (char*) getName(sd_files[i], dir);
        for (y = 0; y < cld_size; y++) {
            if (strncmp(name, cld_files[y], strlen(name)) == 0) {
                newFile = 1;
                break;
            }
        }

        if (newFile == 0) {
            sprintf(add[*n_add], "%s", name);
            *n_add = *n_add + 1;
        }
    }

    for (i = 0; i < cld_size; i++) {
        newFile = 0;
        for (y = 0; y < sd_size; y++) {
            char * name = (char*) getName(sd_files[y], dir);
            if (strncmp(cld_files[i], name, strlen(name)) == 0) {
                newFile = 1;
                break;
            }
        }

        if (newFile == 0) {
            sprintf(supp[*n_supp], "%s", cld_files[i]);
            *n_supp = *n_supp + 1;
        }
    }

    printf("\nNombre de fichiers supprimés : %u\nNombre de fichiers ajoutés : %u\n\n", *n_supp, *n_add);

    printf("Fichiers supprimés :\n\n");

    for (int i = 0; i < *n_supp; i++) {
        printf("%s\n", supp[i]);
    }

    printf("Fichiers ajoutés :\n\n");

    for (int i = 0; i < *n_add; i++) {
        printf("%s\n", add[i]);
    }

    free(dir);
    free(suppName);
}

unsigned int dossiers_to_list (char *** dossiers, char ** list, unsigned int nb_dossiers) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; dossiers[x][y][0] != 0; y++) {
            sprintf(list[item++], "%s", dossiers[x][y]);
        }
        
        x++;
    }

    return item;
}

unsigned int read_cld (char ** cld) {
    system("ls data/images/cloud > data/images/cloud.txt");
    FILE * CLD = fopen("data/images/cloud.txt", "r");
    unsigned int x = 0;

    while (fgets(cld[x], 99, CLD)) {
        enlever_last_car(cld[x++]);
    }

    fclose(CLD);

    return x;
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

