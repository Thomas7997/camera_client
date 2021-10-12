// Transfert en ligne uniquement pour l'envoi des requêtes CURL à partir des fichiers contenus dans le dossier gets/

#include "transferts.h"

// Faire attention à la gestion des erreurs

void supp_last_char (char * buffer) {
    unsigned int len =  strlen(buffer);
    buffer[len-1] = 0;
}

int transferts (Camera * camera, GPContext * context, char ** transferts, unsigned int transfert_choice) {
    char ** cloud = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char * commande = (char*) calloc(250, sizeof(char));
    FILE * HISTORIQUE = fopen("data/images/historique.txt", "a");
    FILE * CLOUD = fopen("data/images/cloud.txt", "r");
    int reqStatus, prevReqStatus; // A utiliser

    do {
        HISTORIQUE = fopen("data/images/historique.txt", "a");

        if (HISTORIQUE == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
        }
    } while (HISTORIQUE == NULL);

    do {
        CLOUD = fopen("data/images/cloud.txt", "a");

        if (CLOUD == NULL) {
            printf ("\n\n\n\n\n\nErreur de lecture de fichier.\n");
        }
    } while (CLOUD == NULL);

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        cloud[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    unsigned int x, nb_lines_tr, nb_lines_tr_cld;

    do {
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
            sprintf(commande, "cp -f data/images/gets/%s /home/remote/camera_server/public;rm -f data/images/gets/%s", transferts[x], transferts[x]);
            system(commande);
            printf ("%s\n", commande);

            do {
                reqStatus = send_request(transferts[x]);
                handle_error_net_status(reqStatus);
            } while (reqStatus != 0);

            // Stoquer dans historique.txt
            fprintf(HISTORIQUE, "%s\n", transferts[x]);
        }
    } while (transfert_choice > 0);

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        free(cloud[i]);
    }

    free(commande);
    free(cloud);
    fclose(HISTORIQUE);
    fclose(CLOUD);
    
    return 0;
}