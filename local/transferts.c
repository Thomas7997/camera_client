// Transfert en ligne uniquement pour l'envoi des requêtes CURL à partir des fichiers contenus dans le dossier gets/

#include "net.h"

// Faire attention à la gestion des erreurs

void supp_last_char (char * buffer) {
    unsigned int len =  strlen(buffer);
    buffer[len-1] = 0;
}

int main (void) {
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** cloud = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char * commande = (char*) calloc(250, sizeof(char));
    FILE * TRANSFERTS;
    FILE * STOP;
    FILE * CLOUD;
    FILE * HISTORIQUE = fopen("data/images/historique.txt", "a");
    int stop = -1;
    int reqStatus, prevReqStatus; // A utiliser

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        cloud[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    unsigned int x, nb_lines_tr, nb_lines_tr_cld;

    do {
        system("ls data/images/gets > data/images/gets.txt");
        system("ls data/images/cloud > data/images/cloud.txt");
        STOP = fopen("data/stop/selection.txt", "r");
        TRANSFERTS = fopen("data/images/gets.txt", "r");
        CLOUD = fopen("data/images/cloud.txt", "r");

        fscanf(STOP, "%d", &stop);

        for (unsigned int t = 0; t < MAX_CAPTURES; t++) {
            strcpy(transferts[t], "");
            strcpy(cloud[t], "");
        }

        x = 0;
        
        while (fgets(transferts[x++], TAILLE_NOM, TRANSFERTS));
        nb_lines_tr = x;
        x = 0;

        while (fgets(cloud[x++], TAILLE_NOM, CLOUD));
        nb_lines_tr_cld = x;
        x = 0;

        // Les lignes commentées seront executées sur le RPI uniquement

        // On  transfert en premier les images sauvegardées durant le mode hors ligne

        for (x = 0; x < nb_lines_tr_cld-1; x++) {
            strcpy(commande, "");
            supp_last_char(cloud[x]);
            sprintf(commande, "cp -f data/images/cloud/%s /home/thomas/camera_server/public;rm -f data/images/cloud/%s", cloud[x], cloud[x]);
            system(commande);
            printf ("%s\n", commande);

            do {
                reqStatus = send_request(cloud[x]);
                handle_error_net_status(reqStatus);
            } while (reqStatus < 0);

            fprintf(HISTORIQUE, "%s\n", cloud[x]);
        }

        for (x = 0; x < nb_lines_tr-1; x++) {
            strcpy(commande, "");
            supp_last_char(transferts[x]);
            sprintf(commande, "cp -f data/images/gets/%s /home/thomas/camera_server/public;rm -f data/images/gets/%s", transferts[x], transferts[x]);
            system(commande);
            printf ("%s\n", commande);

            do {
                reqStatus = send_request(transferts[x]);
                handle_error_net_status(reqStatus);
            } while (reqStatus != 0);

            fprintf(HISTORIQUE, "%s\n", transferts[x]);
        }

        fclose(STOP);
        fclose(TRANSFERTS);
        fclose(CLOUD);
    } while (stop == 0);

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        free(transferts[i]);
        free(cloud[i]);
    }

    free(transferts);
    free(commande);
    free(cloud);
    fclose(HISTORIQUE);
    
    return 0;
}