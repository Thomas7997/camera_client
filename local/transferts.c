#include "manage_selected2.h"

// Transfert en ligne uniquement pour l'envoi des requêtes CURL

int main (void) {
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    FILE * TRANSFERTS;
    FILE * STOP;
    int stop = -1;

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        transferts[i] = (char*) calloc(TAILLE_NOMS, sizeof(char));
    }

    unsigned int x;

    do {
        STOP = fopen("data/stop/selection.txt", "r");
        TRANSFERTS = fopen("data/images/transferts.txt", "r");
        fscanf(STOP, "%d", &stop);
        x = 0;

        if (stop == 0) {
            while (fgets(transferts[x++], TAILLE_NOMS, TRANSFERTS));

        }
        
        else if (stop == 1) {
            fclose(STOP);
            break;
        }

        else {
            return 1;
        }

        fclose(STOP);
        fclose(TRANSFERTS);
    } while (stop == 0);

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        free(transferts[i]);
    }

    free(transferts);
    
    return 0;
}