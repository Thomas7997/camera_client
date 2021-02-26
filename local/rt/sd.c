#include "sd.h"

extern int x_sd;
extern int dir_nb_sd;

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