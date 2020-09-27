/* ALGO POUR PARSER LA NOTATION D'UNE IMAGE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int rating (char ** lines, int size) {
    unsigned int i, x, y;
    int result;

    char * buf = calloc(100, sizeof(char));

    for (i = 0; i < size; i++) {
        x = 0;
        y = 0;

        while (lines[i][x] != ':') {
            buf[y++] = lines[i][x++];
        }

        x++;

        if (strncmp(buf, "Rating", 6) == 0) {
            y = 0;
            strcpy(buf, "");
            while (lines[i][x] != 0) {
                buf[y++] = lines[i][x++];
            }

            y = strlen(lines[i]);

            // sscanf(lines[i], "%d", &result);
            sscanf(buf, "%d", &result);
            return result;
            break;
        }

        strcpy(buf, "");
    }

    free(buf);
    return -1;
}

int main (void) {
    const char * path = "./model/rating.txt"; // Chemin vers le résultat de exif

    FILE * RATING = fopen(path, "r");

    char ** lines = calloc(10, sizeof(char*));

    for (int i = 0; i < 10; i++) {
        lines[i] = calloc(100, sizeof(char));
    }

    int x = 0;

    while (fgets(lines[x++], 99, RATING));

    printf ("RATING : %d\n", rating(lines, x));

    for (int i = 0; i < 10; i++) {
        free(lines[i]);
        lines[i] = NULL;
    }

    free(lines);
    lines = NULL;
    fclose(RATING);

    return 0;
}