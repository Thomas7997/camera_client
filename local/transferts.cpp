// Transfert en ligne uniquement pour l'envoi des requêtes CURL à partir des fichiers contenus dans le dossier gets/

#include "manage_selected2.h"

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
    int stop = -1;

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
            sprintf(commande, "mv data/images/cloud/%s /home/thomas/camera_server/public", cloud[x]);
            // system(commande);
            printf ("%s\n", commande);
            // send_request(cloud[x]);
        }

        for (x = 0; x < nb_lines_tr-1; x++) {
            strcpy(commande, "");
            supp_last_char(transferts[x]);
            sprintf(commande, "mv data/images/gets/%s /home/thomas/camera_server/public", transferts[x]);
            // system(commande);
            printf ("%s\n", commande);
            // send_request(transferts[x]);
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
    
    return 0;
}