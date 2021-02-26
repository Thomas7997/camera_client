// AUTO VIDEO
#include "auto.h"
#include "connexions.h"

int video_auto (Camera * camera, GPContext * context, char ** transferts, unsigned int * nb_transferts, int * nb, char ** liste_captures, unsigned int * nb_list) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    char ** videos = (char**) calloc(MAX_CAPTURES, sizeof(char*));
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
        videos[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    unsigned int files_nb = 0, new_file_index = 0, nb_files = 0, x;

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

    nb_medias = getVideoDatas(dossiers, dirs_n, videos, files, liste_captures, &files_nb, camera, context, dir_sizes);

    printf("%d\n", nb_medias);

    printf("VIDEOS SELECTED\n");

    if (nb_medias < 0) return nb_medias; // Code d'erreur

    if (*nb == 0) {
        *nb_list = save_clist_slist(liste_captures, videos, nb_medias, *nb_list);
    }

    int ret_comp = compareFilesLists(transferts, videos, liste_captures, nb_medias, *nb_list, files_index_list, camera, context);

    printf("LIST COMPARED\n");

    if (ret_comp < 0) return ret_comp; // Code d'erreur

    x = 0;

    if (ret_comp != 0) {
        // Store list
        *nb_list = save_clist_slist(liste_captures, videos, nb_medias, *nb_list); // curent to stored   
        *nb_transferts = ret_comp;
    }

    printf("LIST SAVED\n");

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(videos[i]);
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

    free(videos);
    free(dirs_n);
    free(dir_sizes);
    free(dossiers);
    free(files);
    free(files_index_list);

    return 1;
}
