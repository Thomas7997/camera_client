#include <cstdlib>
#include <cstdio>
#include "manage_selected2.h"

int main (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int *) calloc(1000, sizeof(unsigned int));

    for (int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (int i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    int successBoot = 0;

    FILE * CONNECTED;
    FILE * WIFI;
    int wifiStatus = 0;

    // DÉBUT RÉPÉTITIONS

    int stop = -1;
    do {
        do {
            gp_camera_new (&camera);
            status = gp_camera_init(camera, context);
            handleError(status);

            if (status < 0) {
                generateError(status);
                // gp_camera_exit(camera, context);
                gp_camera_free(camera);
                successBoot = 0;
                CONNECTED = fopen("data/tmp/camera_connected.txt", "w");
                fprintf(CONNECTED, "0");
                fclose(CONNECTED);
            }

            usleep(5000);
        } while (status != 0);

        if (successBoot == 0) {
            // Indiquer que la connexion est établie correctement
            CONNECTED = fopen("data/tmp/camera_connected.txt", "w");
            fprintf(CONNECTED, "1");
            fclose(CONNECTED);

            WIFI = fopen("data/tmp/wifi_status.txt", "r");
            fscanf(WIFI, "%d", &wifiStatus);

            status = getCameraModel(camera);

            generateError(0);
            successBoot = 1;

            if (status < 0) continue;
        }

        int i, j, number = 0;
        unsigned int files_nb = 0, transferts_nb = 0;
        FILE * STOP = fopen("data/stop/selection.txt", "r");
        if (STOP == NULL) return 1;
        fscanf(STOP, "%d", &stop);
        if (stop == -1) return 1;

        i = 0;

        for (unsigned int e = 0; e < MIN_DIRS; e++) {
            for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
                strcpy(dossiers[e][j], "");
            }            
        }

        for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
            strcpy(transferts[e], "");
        }
        
        status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

        if (status < 0) continue;

        status = eachFileRating(dossiers, dirs_n, transferts, dir_sizes, files_nb, &transferts_nb, camera, context);

        if (status < 0) continue;

        printf ("Mode en ligne.\n");
        status = transferer_noms(transferts, transferts_nb, context, camera, wifiStatus);
        if (status < 0) continue;

        fscanf(STOP, "%d", &stop);
        fclose(STOP);

        gp_camera_exit(camera, context);
        gp_camera_free(camera);
    } while (stop != 1);

    // Vider le fichier car il n'y a aucun modèle
    FILE * CAM_MODEL = fopen("data/tmp/camera.txt", "w");
    fclose(CAM_MODEL);

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

    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);

    return 0;
}
