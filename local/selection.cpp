#include "manage_selected2.h"
#include <cstdlib>
#include <cstdio>

// unsigned int somme (int * buffer) {
//     unsigned int sommeVal = 0;
//     while (buffer[i] != 0) sommeVal += buffer[i];
//     return sommeVal;
// }

int getDay (void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday;
}

void RemplirLignes (char ** lns1, char ** lns2) {
    unsigned int i = 0;

    while (lns2[i][0] != 0) lns1[i] = lns2[i];
    return;
}

void divisionListes (char *** decoupages, char ** liste, unsigned int liste_size) {
    unsigned int i = 0, size, day, current_day, newDay = 1;
    char ** lines = (char**) calloc(10000, sizeof(char*));
    char * commande = (char*) calloc(100, sizeof(char));
    char * path = (char*) calloc(100, sizeof(char));

    for (int d = 0; d < 10000; d++) {
        lines[d] = (char*) calloc(10, sizeof(char));
    }

    FILE * DAYS = fopen("./data/tmp/days.txt", "a+");

    while (fgets(lines[i++], 9, DAYS));
    size = i;
    i = 0;

    current_day = getDay();

    // Comparer les dates de jour

    for (i = 0; i < size; i++) {
        sscanf(lines[i], "%d", &day);
        
        if (day != current_day) {
            newDay = 0;
            break;
        }
    }

    //+

    if (newDay && decoupages[0][0][0] == 0) {
        sprintf(path, "data/tmp/days/%d.txt", current_day);
        strcat(commande, path);
        FILE * DAY = fopen(path, "a+");
        // Première division

        RemplirLignes(decoupages[0], liste); // Anciens fichiers

        // Capturer le maximum

        fclose(DAY);
    }

    else {
        FILE * DAILYLIST = fopen(path, "r");

        // Ajouter les nouveaux fichiers de la SD à decoupages[1] car decoupages[0] reste constant

        // Recevoir le maximum

        fclose(DAILYLIST);
    }

    for (int d = 0; d < 10000; d++) {
        free(lines[d]);
    }

    free(commande);
    free(path);
    free(lines);
    fclose(DAYS);
}

int main (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    char ** images_list = (char**) calloc(PART_NB, sizeof(char*));

    for (unsigned int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        images_list[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    int stop = -1;
    do {
        do {
            gp_camera_new (&camera);
            status = gp_camera_init(camera, context);
            handleError(status);

            if (status < 0) {
                generateError(status);
                gp_camera_exit(camera, context);
                gp_camera_free(camera);
            }

            usleep(5000);
        } while (status != 0);

        int i, j, number = 0;

        i = 0;

        FILE * STOP = fopen("data/stop/selection.txt", "r");
        
        // if (STOP == NULL) return 1;
        // fscanf(STOP, "%d", &stop);
        // if (stop == -1) return 1;

        unsigned int files_nb = 0, transferts_nb = 0;

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
        
        status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

        if (status < 0) continue;

        unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

        // if (nb_files > 50) {
        //     nb_files = 50;
        // }

        // cut_list(files, nb_files, images_list);

        printf ("1\n");
        status = eachFileRating_2(files, transferts, nb_files, &transferts_nb, camera, context);
        handleError(status);
        if (status < 0) break;

        // status = eachFileRating(dossiers, dirs_n, transferts, dir_sizes, files_nb, &transferts_nb, camera, context);

        // if (status < 0) continue;

        // status = transferer_noms(transferts, transferts_nb, context, camera);

        // if (status < 0) continue;

        fscanf(STOP, "%d", &stop);
        fclose(STOP);

        gp_camera_exit(camera, context);
        gp_camera_free(camera);
    } while (stop != 1);

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
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

    for (unsigned int i = 0; i < PART_NB; i++) {
        free(images_list[i]);
    }

    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);
    free(files);
    free(images_list);

    return 0;
}