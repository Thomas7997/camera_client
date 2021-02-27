#include "selection_optimale.h"
#include "manage_selected.h"

// Segmentation fault au bout de beaucoup d'executions

int selection_optimale (Camera * camera, GPContext * context, char ** transferts, unsigned int * transferts_nb, unsigned int * command, unsigned int * freed, char *** dossiers, char ** dirs_n, unsigned int * dir_sizes, char ** files, char ** images_list, char ** transferts_tmp, char ** photos) {
    int status = 0;

    // DÉBUT RÉPÉTITIONS

    int i, j, number = 0;

    i = 0;

    unsigned int files_nb = 0;

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(transferts[e], "");
    }

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(files[e], "");
    }

    for (unsigned int e = 0; e < PART_NB; e++) {
        strcpy(images_list[e], "");
    }

    int nb_files  = 0;
    
    status = get_files(files, camera, context, &files_nb);

    if (status < 0) return status;

    nb_files = filterPhotos (photos, files, files_nb);

    cut_list(photos, nb_files, images_list);

    status = eachFileRating_1(images_list, transferts_tmp, nb_files, transferts_nb, camera, context);
    
    if (status < 0) return status;

    printf("Transférer les noms.\n");
    status = transferer_noms (transferts_tmp, *transferts_nb, context, camera);

    if (status < 0) return status;

    printf("Transferts terminés.\n");

    *transferts_nb = status;

    // Copier transferts_tmp à transferts là

    // int online = 0;

    // FIN RÉPÉTITIONS

    return 1;
}
