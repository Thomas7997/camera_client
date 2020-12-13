#include "manage_selected2.h"
#include <cstdlib>
#include <cstdio>

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

unsigned int getImageNumber (char * buffer) {
    unsigned int x = 0, y = strlen(buffer), nb;
    char * nb_str = (char*) calloc(100, sizeof(char));

    while (buffer[y--] != '.');
    while (buffer[y] != '_') {
        nb_str[x++] = buffer[y--];
    }

    mirroir(nb_str, x);
    sscanf(nb_str, "%u", &nb);
    free(nb_str);

    return nb;
}

int compareFilesLists(char ** transferts, char ** files, char ** liste_captures, int files_nb, int nb_list, unsigned int *new_file_index) {
    unsigned int i = 0, y = 0, newP, x = 0, a = 0;

    for (i = 0; i < files_nb; i++) {
        newP = 0;
        for (y = 0; y < nb_list; y++) {
            if (strcmp(files[i], liste_captures[y]) == 0) {
                newP = 1;
                break;
            }
        }

        if (newP == 0) {
            printf("%s\n", files[i]);
            strcpy(transferts[a++], files[i]); 
        }

        printf ("%d\n", getImageNumber(files[i]));
    }

    return a;
}

/*

    for (unsigned int tran = 0;tran < size; tran++) {
        cmp = 0; 
        for(unsigned int his = 0;his < size1; his++) {
            if(strncmp(historique1[his], transferts[tran], strlen(historique1[his])) == 0) {
                cmp = 1;
                break;
            }
        }

        if(cmp == 0) {
            printf("TRANSFERER : %s\n", transferts[tran]);
            strcpy(HistoRet[a++], transferts[tran]);
            break;
        }
    }

*/

int save_clist_slist(char ** liste_captures, char ** files, unsigned int files_nb, int nb_list) {
    unsigned int i = 0;
    
    for (i = 0; i < files_nb - nb_list; i++) {
        strcpy(liste_captures[nb_list+i], files[nb_list+i]);
    }
}

void afficher_liste(char ** transferts, unsigned int ret_comp) {
    unsigned int x = 0;

    while (x < ret_comp) {
        printf ("%s\n", transferts[x]);
        x++;
    }
}

// Ce programme s'applique aux photos et aux videos

// Il faudra mettre en place unn filtre dans les deux

int main (void) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));
    FILE * W = fopen("medias.txt", "a+");

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*)); // Liste mémoire
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));

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
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    int stop = -1;

    unsigned int files_nb = 0, nb_list = 0, new_file_index = 0, nb_files = 0;

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
        
        status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);

        if (status < 0) continue;

        nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);

        unsigned int ret_comp = compareFilesLists(transferts, files, liste_captures, nb_files, nb_list, &new_file_index);

        if (ret_comp != 0 && nb_list != 0) {
            // Store list
            nb_list = save_clist_slist(liste_captures, files, nb_files, nb_list); // curent to stored   
        
            // Command transfert on new file
            fprintf (W, "Transferer %s\n", files[new_file_index]);
        }

        afficher_liste(transferts, ret_comp);

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

    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);
    free(files);
    fclose(W);

    return 0;
}
