#include "auto.h"

unsigned int filterVideos (char ** videos_l, char ** files_l, unsigned int files_n) {
    unsigned int x = 0, i, j, y;
    char ext[5] = "";

    for (i = 0; i < files_n; i++) {
        strcpy(ext, "");
        j = strlen(files_l[i]) - 1;
        y = 0;
        while (files_l[i][j] != '.') {
            ext[y++] = files_l[i][j--];
        }

        if (strcmp(ext, "VOM") == 0 || strcmp(ext, "4PM") == 0) {
            // Il s'agit donc d'une vidéo
            strcpy(videos_l[x++], files_l[i]);
            printf("%s\n", files_l[i]);
        }
    }

    return x;
}

unsigned int filterPhotos (char ** photos_l, char ** files_l, unsigned int files_n) {
    unsigned int x = 0, i, j, y;
    char ext[5] = "";

    for (i = 0; i < files_n; i++) {
        strcpy(ext, "");
        j = strlen(files_l[i]) - 1;
        y = 0;
        while (files_l[i][j] != '.') {
            ext[y++] = files_l[i][j--];
        }

        if (strcmp(ext, "GPJ") == 0) {
            // Il s'agit donc d'une vidéo
            strcpy(photos_l[x++], files_l[i]);
        }
    }

    return x;
}

unsigned int getImageNumber (char * buffer) {
    unsigned int x = 0, y = strlen(buffer), nb;
    char * nb_str = (char*) calloc(100, sizeof(char));

    while (buffer[y--] != '.');
    while (buffer[y] != '_') {
        nb_str[x++] = buffer[y--];
    }

    mirroir(nb_str, x);
    sscanf(nb_str, "%u", &nb);
    free(nb_str);

    return nb;
}

int compareFilesLists(char ** transferts, char ** files, char ** liste_captures, int files_nb, int nb_list, unsigned int * indexs, Camera * camera, GPContext * context) {
    unsigned int i = 0, y = 0, newP, x = 0, a = 0;

    for (i = 0; i < files_nb; i++) {
        newP = 0;
        for (y = 0; y < nb_list; y++) {
            if (strcmp(files[i], liste_captures[y]) == 0) {
                newP = 1;
                break;
            }
        }

        if (newP == 0) {
            printf("\n\n\n\n\n\n\n\n\nT : %s\n%d\n", files[i], i);
            sprintf(transferts[a], "%s", files[i]);
            // Gérer le prochain return
            transferer_nom_auto(transferts[a], context, camera);
            printf("%s\n", transferts[a++]);
        }
    }

    return a;
}

unsigned int save_clist_slist(char ** liste_captures, char ** files, unsigned int files_nb, unsigned int nb_list) {
    unsigned int i = 0;
    
    for (i = 0; i < files_nb; i++) {
        strcpy(liste_captures[i], files[i]);
    }

    return files_nb;
}

void afficher_liste(char ** transferts, unsigned int ret_comp) {
    unsigned int x = 0;

    while (x < ret_comp) {
        printf ("%s\n", transferts[x]);
        x++;
    }
}

int getVideoDatas (char ** videos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes) {
    int status = get_files(files, camera, context, files_nb);

    if (status < 0) return status;

    // La matière sortante est la liste de vidéos totale
    return filterVideos(videos, files, *files_nb);
}

int getPhotoDatas (char ** photos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes) {
    int status = get_files(files, camera, context, files_nb);

    if (status < 0) return status;

    // La matière sortante est la liste de vidéos totale
    return filterPhotos(photos, files, *files_nb);
}