#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (void) {
    FILE * ERR;
    FILE * STOP_S;
    FILE * STOP_A;
    int status = 0, previousStatus = 0, stop_s = -1, stop_a = -1;

    do {
        // Analyse
        ERR = fopen("data/tmp/errors.txt", "r");
        fscanf(ERR, "%d", &status);

        if (previousStatus == 0) {
            previousStatus = status;
        }

        else if (previousStatus != status) {
            // Notifier
            printf ("Notifier %d\n", status);
            previousStatus = status;
        }

        // Conditions d'arrêt
        STOP_S = fopen("data/stop/selection.txt", "r");
        STOP_A = fopen("data/stop/transferts.txt", "r");

        fscanf(STOP_S, "%d", &stop_s);
        fscanf(STOP_A, "%d", &stop_a);

        fclose(STOP_A);
        fclose(STOP_S);
        fclose(ERR);

        usleep(50000);
    } while (stop_a == 0 || stop_s == 0);
    
    
    return 0;
}