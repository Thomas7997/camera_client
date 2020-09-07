#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    int command = -1;

    while (1) {
        FILE * CAPTURE = fopen("commands/capture.txt", "r");

        int last = 0, command = -1;

        fscanf(CAPTURE, "%d", &command);
        printf ("%d\n", command);

        if (command == 1) {
            // Sauvegarder le LOG

            // Effectuer l'action
            if (last == 0) {
                last = command;
                system("./capture");
                printf("LANCÉ.\n");
            }
        }

        else {

        }

        fclose(CAPTURE);
        sleep(1);
    }

    return 0;
}