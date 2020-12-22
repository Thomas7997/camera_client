// AUTO PHOTO ET VIDEO
#include "auto.h"
#include "connexions.h"

// Ce programme s'applique aux photos et aux photos

// Il faudra mettre en place un filtre dans les deux

int photo_auto (Camera * camera, GPContext * context, char ** transferts, unsigned int * nb_transferts, unsigned int * command, unsigned int * freed, int * nb) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*)); // Liste mémoire
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    char ** photos = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    unsigned int * files_index_list = (unsigned int*) calloc(MAX_CAPTURES, sizeof(unsigned int));
    unsigned int nb_medias;

    for (unsigned int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        photos[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    unsigned int files_nb = 0, nb_list = 0, new_file_index = 0, nb_files = 0, x;

    // Phase initiale où le programme démarre et aucune photo n'est transferée

    int i, j, number = 0;
    i = 0;

    for (unsigned int e = 0; e < MIN_DIRS; e++) {
        for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
            strcpy(dossiers[e][j], "");
        }            
    }

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(transferts[e], "");
    }

    for (unsigned int e = 0; e < MAX_CAPTURES*MIN_DIRS; e++) {
        strcpy(files[e], "");
    }

    printf("1\n");
    nb_medias = getPhotoDatas(dossiers, dirs_n, photos, files, liste_captures, &files_nb, camera, context, dir_sizes);
    printf("2\n");

    if (*nb == 0) {
        nb_list = save_clist_slist(liste_captures, photos, nb_medias, nb_list);
    }

    printf("3\n");
    unsigned int ret_comp = compareFilesLists(transferts, photos, liste_captures, nb_medias, nb_list, files_index_list);
    x = 0;
    printf("4\n");

    printf ("%d\n%d\n", ret_comp, nb_list);
    if (ret_comp != 0) {
        // Store list
        nb_list = save_clist_slist(liste_captures, files, nb_medias, nb_list); // curent to stored   
    
        printf("%d\n", ret_comp);
        while (x < ret_comp) {
            // Command transfert on new file
            printf("%s\n", transferts[x++]);
        }
    }

    *nb_transferts = x;

    // afficher_liste(transferts, ret_comp);

    // if (nb == 0) {
    //     free_usb(camera, context);
    // }

    *freed = 0;
    *command = 1;

    usleep(500000);

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(photos[i]);
    }

    for (int d = 0; d < MIN_DIRS; d++) {
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            free(dossiers[d][dy]);
        }
        free(dossiers[d]);
        free(dirs_n[d]);
    }

    for (int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        free(files[i]);
    }

    free(photos);
    free(dirs_n);
    free(liste_captures);
    free(dir_sizes);
    free(dossiers);
    free(files);
    free(files_index_list);

    return 0;
}
