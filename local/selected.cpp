#include <cstdlib>
#include <cstdio>
#include "manage_selected2.h"

int main (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    Camera * camera;
    GPContext *context = sample_create_context();
    gp_camera_new (&camera);

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

    // DÉBUT RÉPÉTITIONS

    int stop = -1;
    do {
        do {
            status = gp_camera_init(camera, context);
            handleError(status);
        } while (status != 0);

        int i, j, number = 0;

        i = 0;

        FILE * STOP = fopen("data/stop/selection.txt", "r");
        
        if (STOP == NULL) return 1;
        fscanf(STOP, "%d", &stop);
        if (stop == -1) return 1;

        int files_nb = 0, transferts_nb = 0;

        for (unsigned int e = 0; e < MIN_DIRS; e++) {
            for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
                strcpy(dossiers[e][j], "");
            }            
        }

        for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
            strcpy(transferts[e], "");
        }
        
        files_nb = get_files_and_dirs(dossiers, dirs_n, camera, context);
        transferts_nb = eachFileRating(dossiers, dirs_n, transferts, dir_sizes, files_nb, camera, context);
        transferer_noms(transferts, transferts_nb, context, camera);
        fscanf(STOP, "%d", &stop);
        fclose(STOP);

        gp_camera_exit(camera, context);
    } while (stop != 1);

    // FIN RÉPÉTITIONS

    gp_camera_free(camera);
    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);

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

    return 0;
}
