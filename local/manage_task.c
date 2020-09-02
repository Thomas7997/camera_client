#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    int command = -1;

    while (1) {
        FILE * MANAGE = fopen("commands/manage.txt", "r");

        fscanf(MANAGE, "%d", &command);
        printf ("%d\n", command);
        if (command == 1) {
            // SAUVEGARDER LE LOG
        
            // EFFECTUER L'ACTION
            system("./manage");
        }

        else if (command == 0) {
            // SAUVEGARDER LE LOG
        }

        else {
            // ECRIRE LE FICHIER
            system("echo \"0\" > commands/manage.txt");
        }

        fclose(MANAGE);

        usleep(5000);
    }

    return 0;
}