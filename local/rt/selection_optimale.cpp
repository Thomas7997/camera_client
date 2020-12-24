#include "selection_optimale.h"
#include "manage_selected.h"

// unsigned int somme (int * buffer) {
//     unsigned int sommeVal = 0;
//     while (buffer[i] != 0) sommeVal += buffer[i];
//     return sommeVal;
// }

// int getDay (void) {
//     time_t t = time(NULL);
//     struct tm tm = *localtime(&t);
//     return tm.tm_mday;
// }

// void RemplirLignes (char ** lns1, char ** lns2) {
//     unsigned int i = 0;

//     while (lns2[i][0] != 0) lns1[i] = lns2[i];
//     return;
// }

int selection_optimale (Camera * camera, GPContext * context, char ** transferts, unsigned int * transferts_nb, unsigned int * command, unsigned int * freed, char *** dossiers, char ** dirs_n, unsigned int * dir_sizes, char ** files, char ** images_list) {
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
    
    printf("1\n");

    status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

    if (status < 0) return status;

    unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

    cut_list(files, nb_files, images_list);

    status = eachFileRating_1(images_list, transferts, nb_files, transferts_nb, camera, context);
    if (status < 0) return status;

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
