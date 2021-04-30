#include "sd2.h"

int sd_card_lecture_mode (char ** files, Camera * camera, GPContext * context) {
    unsigned int nb_files = 0;
    int status = get_files (files, camera, context, &nb_files);
    printf("%d\n", status);
    if (status < 0) handleError(status);

    printf ("LECTURE DE LA LISTE TERMINÉE.\n");

    printf("AFFICHAGE DE LA LISTE ...\n");

    for (int i = 0; i < nb_files; i++) {
        printf("%s\n", files[i]);
    }

    printf("AFFICHAGE DE LA LISTE TERMINÉ.\n");

    status = get_sd_card_previews (files, nb_files, camera, context);
    handleError(status);

    if (status < 0) return status;

    return 0;
}

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(100, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    printf("%d\n", nb);
    for (i = 0; i < nb; i++) {
        char * filename = (char*) getName(files[i], dir);
        printf("%s\n%s\n", dir, filename);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        // sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);
        sprintf(targetPath, "../data/images/cloud/%s", filename); // Need another script to move files when wifiConnected is ON
        status = gp_file_save(file, (const char*) targetPath);

        printf("SAUVEGARDE ...\n");

        if (status < 0) return status;
    }

    free(dir);
    free(targetPath);
    gp_file_free(file);

    return 0;
}

void diff_sd_list_refresh (char ** supp, char ** add, char ** cld_files, char ** sd_files, unsigned int cld_size, unsigned int sd_size, unsigned int * n_add, unsigned int * n_supp) {
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
            sprintf(add[*n_add], "%s", sd_files[i]);
            *n_add = *n_add + 1;
        }
        free(name);
    }

    for (i = 0; i < cld_size; i++) {
        newFile = 0;
        for (y = 0; y < sd_size; y++) {
            char * name = (char*) getName(sd_files[y], dir);
            if (strncmp(cld_files[i], name, strlen(name)) == 0) {
                newFile = 1;
                break;
            }
            free(name);
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

int sd_refresh (char ** files, char ** supp, char ** add, char ** cld_files, Camera * camera, GPContext * context) {
    unsigned int cld_size, sd_size, n_add, n_supp;

    get_files (files, camera, context, &sd_size); // Read

    cld_size = listDir(cld_files, "../data/images/cloud");

    printf("STARTING ANALYSE ...\n");

    diff_sd_list_refresh (supp, add, cld_files, files, cld_size, sd_size, &n_add, &n_supp); // Analyse

    printf("ANALYSE ENDED.");

    return local_refresh (supp, add, n_add, camera, context);
}

int local_refresh(char ** supp, char ** add, unsigned int n_add, Camera * camera, GPContext * context) {
    unsigned int x = 0;
    char * path = (char*) calloc(200, sizeof(char));

    // Removing files from the local "cloud" directory

    for (char * str = *supp; *str != 0; str = *(supp+x), x++) {
        strcpy(path, "");
        sprintf(path, "../data/images/cloud/%s", str);
        removeFile(path);

    }

    // Adding files to the local "cloud" directory with usb transactions

    free(path);

    return get_sd_card_previews(add, n_add, camera, context);
}

