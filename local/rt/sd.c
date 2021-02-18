#include "sd.h"

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

int sd_card_lecture_mode (char *** dossiers, char ** dirs_n, char ** files, Camera * camera, GPContext * context) {
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

    for (unsigned int e = 0; e < MAX_CAPTURES*MIN_DIRS; e++) {
        strcpy(files[e], "");
    }

    status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

    if (status < 0) return status;

    unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

    status = get_sd_card_previews (files, nb_files, camera, context);

    if (status < 0) return status;

    free(dir_sizes);

    return 1;
}
