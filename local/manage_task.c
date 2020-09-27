#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    int command = -1;

    while (1) {
        FILE * MANAGE = fopen("commands/manage.txt", "r");
        FILE * ARG = fopen("commands/arg.txt", "r");

        char arg = calloc(100, sizeof(char));
        int auto = 0;

        if (strcmp(arg, "select") == 0) {
            // Transfert par sélection
            system("./manage_selected");
        }

        else if (strcmp(arg, "auto") == 0) {
            // Transfert auto
            system("./manage");
        }

        else {
            // ECRIRE LE FICHIER
            // Attendre que le transfert s'active
        }

        fclose(MANAGE);
        fclose(ARG);
        free(arg);
        usleep(5000);
    }

    return 0;
}