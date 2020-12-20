// AUTO PHOTO ET VIDEO
#include "auto.h"

// Ce programme s'applique aux photos et aux photos

// Il faudra mettre en place un filtre dans les deux

int video_auto (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    FILE * WFILE = fopen("medias.txt", "a");

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*)); // Liste mémoire
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    char ** photos = (char**) calloc(MAX_CAPTURES, sizeof(char*));
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
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        photos[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    int stop = -1;

    unsigned int files_nb = 0, nb_list = 0, new_file_index = 0, nb_files = 0, x, nb = 0;

    // Phase initiale où le programme démarre et aucune photo n'est transferée

    do {
        int i, j, number = 0;

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

        i = 0;

        FILE * STOP = fopen("data/stop/video.txt", "r");
        
        if (STOP == NULL) return 1;
        fscanf(STOP, "%d", &stop);
        if (stop == -1) return 1;

        unsigned int transferts_nb = 0;

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

        nb_medias = getVideoDatas(dossiers, dirs_n, photos, files, liste_captures, &files_nb, camera, context, dir_sizes);

        if (nb == 0) {
            nb_list = save_clist_slist(liste_captures, photos, nb_medias, nb_list);
            afficher_liste(liste_captures, nb_list);
            printf("MEDIAS\n%d\n", nb_list);
            afficher_liste(photos, nb_medias);
        }

        // for (x = 0; x < nb_medias; x++) {
        //     printf ("%s\n", photos[x]);
        // }

        unsigned int ret_comp = compareFilesLists(transferts, photos, liste_captures, nb_medias, nb_list, files_index_list);
        x = 0;

        printf ("%d\n%d\n", ret_comp, nb_list);
        if (ret_comp != 0) {
            // Store list
            nb_list = save_clist_slist(liste_captures, files, nb_medias, nb_list); // curent to stored   
        
            printf("%d\n", ret_comp);
            while (x < ret_comp) {
                // Command transfert on new file
                // fprintf (WFILE, "Transferer %s\n", transferts[x]);
                printf("%s\n", transferts[x++]);
            }
        }

        // afficher_liste(transferts, ret_comp);

        fscanf(STOP, "%d", &stop);
        fclose(STOP);

        gp_camera_exit(camera, context);
        gp_camera_free(camera);
        nb++;
    } while (stop != 1);

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
        free(photos[i]);
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

    free(photos);
    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);
    free(files);
    fclose(WFILE);
    free(files_index_list);

    return 0;
}
