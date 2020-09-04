#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    int command = -1;

    while (1) {
        FILE * CAPTURE = fopen("commands/capture.txt", "r");
        fscanf(CAPTURE, "%d", &command);
        printf ("%d\n", command);
        if (command == 1) {
            // Sauvegarder le LOG

            // Effectuer l'action
            system("./capture");
        }

        else if (command == 0) {
            // Sauvegarder le LOG
        }

        else {
            system("echo \"0\" > commands/capture.txt");
        }

        fclose(CAPTURE);
        sleep(1);
    }
}