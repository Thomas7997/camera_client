#include "selection_optimale.h"
#include "manage_selected.h"

int selection_optimale (Camera * camera, GPContext * context, char ** transferts, unsigned int * transferts_nb, unsigned int * command, unsigned int * freed, char *** dossiers, char ** dirs_n, unsigned int * dir_sizes, char ** files, char ** images_list, char ** transferts_tmp, char ** photos) {
    int status = 0;

    // DÉBUT RÉPÉTITIONS

    int i, j, number = 0;

    i = 0;

    unsigned int files_nb = 0;

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

    for (unsigned int e = 0; e < PART_NB; e++) {
        strcpy(images_list[e], "");
    }

    
    status = recursive_directory(dossiers, camera, "/", context, &files_nb);

    if (status < 0) return status;

    unsigned int nb_files = dossiers_to_list(dossiers, files, files_nb);

    nb_files = filterPhotos (photos, files, nb_files);

    cut_list(photos, nb_files, images_list);

    status = eachFileRating_1(images_list, transferts_tmp, nb_files, transferts_nb, camera, context);
    if (status < 0) return status;

    status = transferer_noms (transferts_tmp, *transferts_nb, context, camera);

    if (status < 0) return status;

    *transferts_nb = status;

    // Copier transferts_tmp à transferts là

    // int online = 0;

    if (status < 0) return status;

    printf("1\n");

    // FIN RÉPÉTITIONS

    printf("1\n");

    return 1;
}
