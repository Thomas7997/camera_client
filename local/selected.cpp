#include <cstdlib>
#include <cstdio>
#include "manage_selected2.h"

int main (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    Camera * camera;
    GPContext *context = sample_create_context();

    do {
        gp_camera_new (&camera);
        status = gp_camera_init(camera, context);
        handleError(status);
    } while (status != 0);

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int *) calloc(1000, sizeof(unsigned int));

    int i, j, number = 0;

    for (int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    i = 0;

    unsigned int files_nb = get_files_and_dirs(dossiers, dirs_n, camera, context);
    int transferts_nb = eachFileRating(dossiers, dirs_n, transferts, dir_sizes, files_nb, camera, context);

    transferer_noms(transferts, transferts_nb, context, camera);

    for (i = 0; i < MAX_CAPTURES; i++) {
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

    gp_camera_exit(camera, context);
    gp_camera_free(camera);
    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);

    return 0;
}