#include "sd.h"

int main (void) {
    char *** dossiers = (char***) calloc(1000, sizeof(char**));
    int status = 0;

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(10000, sizeof(char*));
    char ** transferts = (char**) calloc(10000, sizeof(char*));
    char ** dirs_n = (char**) calloc(1000, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int *) calloc(1000, sizeof(unsigned int));
    char ** files = (char**) calloc(1000*10000, sizeof(char*));

    for (int d = 0; d < 1000; d++) {
        dossiers[d] = (char**) calloc(10000, sizeof(char*));
        for (int dy = 0; dy < 10000; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (int i = 0; i < 10000; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < 1000*10000; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    do {
        gp_camera_new (&camera);
        status = gp_camera_init(camera, context);
        
        if (status < 0) {
            printf("%d\n", status);
        }

        usleep(5000);
    } while (status != 0);

    unsigned int files_nb;

    status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);
    printf("Fichiers lus\n");

    if (status < 0) printf("%d\n", status);

    unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

    status = get_sd_card_previews (files, nb_files, camera, context);

    if (status < 0) printf("%d\n", status);

    // FIN RÉPÉTITIONS

    for (int i = 0; i < 10000; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
    }

    for (int d = 0; d < 1000; d++) {
        for (int dy = 0; dy < 10000; dy++) {
            free(dossiers[d][dy]);
        }
        free(dossiers[d]);
        free(dirs_n[d]);
    }

    for (int i = 0; i < 10000*1000; i++) {
        free(files[i]);
    }

    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);
    free(files);

    return 0;
}

int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context) {
    CameraList * folderList;
    char * folder = (char*) calloc(100, sizeof(char));
    gp_list_new(&folderList);
    const char * dir;
    int status = gp_camera_folder_list_folders(camera, "/", folderList, context);
    if (status < 0) printf("%d\n", status);

    status = gp_list_get_name(folderList, 0, (const char**) &dir);
    if (status < 0) printf("%d\n", status);

    // status = gp_list_reset(folderList);

    char * tmp = (char*) calloc(100, sizeof(char));
    char * tmp_dir = (char*) calloc(100, sizeof(char));

    CameraList * fileList;
    gp_list_new(&fileList);

    unsigned int n = strlen(dir)-1;
    strcpy(tmp_dir, "/store_");

    unsigned int x = 0, z = 0;

    while (dir[z++] != '_');
    while (dir[z] != 0) {
        tmp_dir[7+x++] = dir[z++];
    }

    sprintf(folder, "%s/DCIM", tmp_dir);

    // printf ("%s\n", folder);

    status = gp_camera_folder_list_folders(camera,
		folder,
		folderList,
		context 
	);
    if (status < 0) printf("%d\n", status);

    int local_nb = gp_list_count(folderList);
    int nb_files = 0;

    if (local_nb < 0) printf ("%d\n", local_nb);
    *nb = local_nb;

    for (unsigned int i = 0; i < *nb; i++) {
        // status = gp_list_reset(fileList);

        const char * subdir;
        status = gp_list_get_name(folderList, i, (const char**) &subdir);

        sprintf (tmp_dir, "%s/%s", folder, subdir);
        // printf ("%s\n", tmp_dir);

        strcpy(dirs_n[i], tmp_dir);

        status = gp_list_reset(fileList);
        if (status < 0) printf("%d\n", status);

        status = gp_camera_folder_list_files(camera, tmp_dir, fileList, context);
        if (status < 0) printf("%d\n", status);

        nb_files = gp_list_count(fileList);
        if (nb_files < 0) printf("%d\n", nb_files);

        dir_sizes[i] = nb_files;

        for (unsigned int j = 0; j < nb_files; j++) {
            const char * file;
            status = gp_list_get_name(fileList, j, (const char**) &file);
            // 
            if (status < 0) printf("%d\n", status);
            strcpy(dirs_b[i][j], file);
            // printf ("%s\n", dirs_b[i][j]);
        }

        // sprintf(dirs_n[i], "/%s", dir);
    }

    free(tmp_dir);
    free(folder);
    free(tmp);
    gp_list_free(folderList);
    gp_list_free(fileList);

    return 0;
}

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(TAILLE_NOM, sizeof(char));

    for (i = 0; i < nb; i++) {
        printf("1");
        char * filename = (char*) getName(files[i], dir);
        printf("2\n");
        printf("%s\n%s\n", dir, filename);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        status = gp_file_save(file, (const char*) "tmp.jpg");

        printf("SAUVEGARDE ...\n");

        if (status < 0) return status;
    }

    free(dir);
    gp_file_free(file);
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

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; y < nb_files[x]; y++) {
            sprintf(list[item++], "%s/%s", dirs[x], dossiers[x][y]);
        }
        
        x++;
    }

    return item;
}

// status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);