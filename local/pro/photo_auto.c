// AUTO PHOTO
#include "auto.h"
#include "connexions.h"

int photo_auto (Camera * camera, GPContext * context, char ** transferts, unsigned int * nb_transferts, int * nb, char ** liste_captures, unsigned int * nb_list) {
    int status = 0;
    char ** files = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** photos = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    unsigned int * files_index_list = (unsigned int*) calloc(MAX_CAPTURES, sizeof(unsigned int));
    unsigned int nb_medias;

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        photos[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    unsigned int files_nb = 0, new_file_index = 0, nb_files = 0, x;

    // Phase initiale où le programme démarre et aucune photo n'est transferée

    int i, j, number = 0;
    i = 0;

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(transferts[e], "");
    }

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(files[e], "");
    }

    printf("Lecture de la liste de photos ...\n");

    // Segmentation fault généré quand la connexion usb est interrompue à cette fonction
    nb_medias = getPhotoDatas(photos, files, liste_captures, &files_nb, camera, context, dir_sizes);

    if (nb_medias < 0) return nb_medias; // Code d'erreur

    printf("Lecture de la liste de photos finie !\n");

    if (*nb == 0) {
        *nb_list = save_clist_slist(liste_captures, photos, nb_medias, *nb_list);
    }

    printf("Comparaison de la liste obtenue ...\n");

    int ret_comp = compareFilesLists(transferts, photos, liste_captures, nb_medias, *nb_list, files_index_list, camera, context);

    if (ret_comp < 0) return ret_comp; // Code d'erreur

    printf("Comparaison de la liste obtenue finie !\n");

    x = 0;

    if (ret_comp != 0) {
        // Store list
        *nb_list = save_clist_slist(liste_captures, photos, nb_medias, *nb_list); // curent to stored   
        *nb_transferts = ret_comp;
    }

    printf("Script fini !!\n");

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(photos[i]);
    }

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(files[i]);
    }

    free(photos);
    free(dir_sizes);
    free(files);
    free(files_index_list);

    return 0;
}
