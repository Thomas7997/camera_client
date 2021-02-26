#include "sd.h"

int x_sd = -1, dir_nb_sd = 0;

int get_sd_card_previews (char *** dossiers, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(100, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    for (i = 0; i < nb; i++) {
        j = 0;
        while (dossiers[i][j][0] != 0) {
            char * filename = (char*) getName(dossiers[i][j++], dir);
            status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
            printf("RECEPTION...\n");

            if (status < 0) return status;

            // sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);
            sprintf(targetPath, "../data/images/cloud/%s", filename);
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
    else if (countFiles > 0) {
        x_sd += 1;
        y = 0;
    }

    for (int i = 0; i < countFiles; i++) {
        gp_list_get_name (list, i, &newfile); /* only entry 0 needed */
        sprintf(dossiers[x_sd][y++], "%s/%s", folder, newfile);
    }

	gp_list_free (list);
	return GP_OK;
}


int sd_card_lecture_mode (char *** dossiers, char ** dirs_n, Camera * camera, GPContext * context) {
    int status = 0;

    int i, j, number = 0;
    unsigned int * dir_sizes = (unsigned int*) calloc(MIN_DIRS, sizeof(int));
    unsigned int files_nb;

    for (unsigned int e = 0; e < MIN_DIRS; e++) {
        for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
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