#include "sd.h"

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(100, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    for (i = 0; i < nb; i++) {
        char * filename = (char*) getName(files[i], dir);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        // sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);
        sprintf(targetPath, "../data/images/cloud/%s", filename);
        status = gp_file_save(file, (const char*) targetPath);

        printf("SAUVEGARDE ...\n");

        if (status < 0) return status;
        i++;
    }

    free(dir);
    free(targetPath);
    gp_file_free(file);
    return 0;
}

int sd_card_lecture_mode (char *** dossiers, char ** files, Camera * camera, GPContext * context) {
    int status = 0;

    int i, j, number = 0;
    unsigned int * dir_sizes = (unsigned int*) calloc(MIN_DIRS, sizeof(int));
    unsigned int dir_nb = 0;
    unsigned int files_nb = 0;

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(files[e], "");
    }

    status = recursive_directory(files, camera, "/", context, &files_nb);
    handleError(status);

    if (status < 0) return status;

    status = get_sd_card_previews (files, files_nb, camera, context);
    handleError(status);

    if (status < 0) return status;

    free(dir_sizes);

    return 1;
}