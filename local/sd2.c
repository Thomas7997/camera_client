#include "sd2.h"

// Permissions list

FilesPermissions * init_files_permissions (void) {
    FilesPermissions * fp = malloc(1 * sizeof(FilesPermissions));
    fp->path = calloc(100, sizeof(char));
    fp->name = calloc(50, sizeof(char));
    fp = NULL;

    return fp;
}

void display_files_permissions (FilesPermissions * fp) {
    if (fp) {
        printf("Name : %s\n", fp->name);
        printf("Path : %s\n", fp->path);

        if (fp->permissions.read) {
            printf("Read : yes\n");
        }

        if (fp->permissions.del) {
            printf("Delete : yes");
        }

        display_files_permissions(fp->next);
    }
}

FilesPermissions * addqueue_files_permissions (FilesPermissions * fp, char * name, char * path, Permissions permissions) {
    FilesPermissions * P;
    FilesPermissions * Q;
    FilesPermissions * tete;

    P = malloc(sizeof(FilesPermissions));

    strcpy(P->name, name);
    strcpy(P->path, path);
    P->permissions = permissions;
    P->next = NULL;

    if (!fp) {
        return P;
    }

    else {
        Q = fp;
        tete = fp;
        while (Q->next) {
            Q = Q->next;
        }
        Q->next = P;
        return tete;
    }
}

void free_files_permissions (FilesPermissions * fp) {
    free(fp->name);
    fp->name = NULL;
    free(fp->path);
    fp->path = NULL;
    free(fp);
    fp = NULL;
}

// Déclaration ici
int x_sd = -1;

int main (void) {
    Camera * camera;
    GPContext *context = sample_create_context();
    int status = 0, nb_directories;
    char ** files = (char**) calloc(10000, sizeof(char*));

    for (int i = 0; i < 10000; i++) {
        files[i] = (char*) calloc(100, sizeof(char));
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

    FilesPermissions * fp = init_files_permissions ();

    unsigned int files_nb;
    status = get_files(files, camera, context, &files_nb, &fp);
    printf("%d\n", files_nb);

    if (status < 0) handleError(status);

    printf ("LECTURE DE LA LISTE TERMINÉE.\n");

    printf("AFFICHAGE DE LA LISTE ...\n");

    for (int j = 0; j < 10000 && files[j][0] != 0; j++) {
        printf("%s\n", files[j]);
    }

    // display_files_permissions(fp);

    printf("AFFICHAGE DE LA LISTE TERMINÉ.\n");

    printf("%d\n", files_nb);

    // status = get_sd_card_previews (files, files_nb, camera, context);

    // if (status < 0) handleError(status);

    for (int i = 0; i < 10000; i++) {
        free(files[i]);
    }

    free(files);
    free_files_permissions (fp);

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

// Fonction principale pour la lecture de fichiers
int get_files (char ** files, Camera * camera, GPContext * context, unsigned int * x, FilesPermissions ** fp) {
    x_sd = -1;
    int status = recursive_directory(files, camera, "/", context, x);
    handleError(status);

    // Check file info(s)

    for (int f = 0; f < *x; f++) {
        CameraFileInfo info;

        char * dir = calloc(100, sizeof(char));
        char * filename = getName (files[f], dir);

        printf("%s\n%s\n", dir, filename);

        // We will work on the file permissions later (maybe with other versions of gphoto2)

        // gp_camera_file_get_info (camera, dir, filename, NULL, context);

        if (status < 0) return status;

        Permissions permissions;

        // Delete perm

        if (info.file.permissions == GP_FILE_PERM_DELETE) {
            permissions.del = 1;
        }

        else {
            permissions.del = 0;
        }

        // Read perm

        if (info.file.permissions == GP_FILE_PERM_DELETE) {
            permissions.read = 1;
        }

        else {
            permissions.read = 0;
        }

        // Add to files permissions

        // *fp = addqueue_files_permissions(*fp, filename, files[f], permissions);

        free(dir);
        free(filename);
    }

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
    printf("%d\n", countFiles);
    if (countFiles < 0) return countFiles;

    for (int z = 0; z < countFiles; z++) {
        gp_list_get_name (list, z, &newfile);
        sprintf(files[++x_sd], "%s/%s", folder, newfile);
        printf("x : %d\npath : %s\n", x_sd, files[x_sd]);
    }

    // Reset ici
    *x = x_sd;

	gp_list_free (list);
	return GP_OK;
}

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);
    CameraAbilities abilities;

    char * dir = (char*) calloc(100, sizeof(char));
    char * targetPath = (char*) calloc(200, sizeof(char));
    char * targetDirCMD = (char*) calloc(200, sizeof(char));

    status = gp_camera_get_abilities (camera, &abilities);

    if (status < 0) return status;

    sprintf(targetDirCMD, "mkdir \"/home/remote/sd_files/c\" \"/home/remote/sd_files/c/sd\" \"/home/remote/sd_files/c/get\"", abilities.model, abilities.model, abilities.model);

    system(targetDirCMD);

    for (i = 0; i < nb; i++) {
        char * filename = (char*) getName(files[i], dir);
        printf("%s\n%s\n", dir, filename);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        // sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);
        sprintf(targetPath, "/home/remote/sd_files/c/sd/%s", filename);
        status = gp_file_save(file, (const char*) targetPath);

        printf("SAUVEGARDE ...\n");

        printf("%d\n", status);
        if (status < 0) return status;
    }

    free(dir);
    free(targetPath);
    free(targetDirCMD);
    gp_file_free(file);
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

// status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);