#include "selection_optimale.h"
#include "manage_selected.h"

int selection_optimale (Camera * camera, GPContext * context, char ** transferts, unsigned int * transferts_nb, unsigned int * command, unsigned int * freed, char *** dossiers, char ** dirs_n, unsigned int * dir_sizes, char ** files, char ** images_list, char ** transferts_tmp) {
    int status = 0;
    printf("1\n");

    printf("1\n");

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

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(transferts_tmp[e], "");
    }
    
    printf("1\n");

    status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

    if (status < 0) return status;

    unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

    cut_list(files, nb_files, images_list);

    status = eachFileRating_1(images_list, transferts_tmp, nb_files, transferts_nb, camera, context);
    if (status < 0) return status;

    status = transferer_noms (transferts_tmp, transferts, *transferts_nb, context, camera);

    if (status < 0) return generateError(status);

    *transferts_nb = status;

    // status = eachFileRating(dossiers, dirs_n, transferts, dir_sizes, files_nb, &transferts_nb, camera, context);

    // if (status < 0) continue;

    int online = 0;

    if (status < 0) return status;

    printf("1\n");

    // FIN RÉPÉTITIONS

    printf("1\n");

    // *command = 1;
    // *freed = 0;

    return 1;
}
