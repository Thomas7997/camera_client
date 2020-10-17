#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void) {
    char ** lines = calloc(1000, sizeof(char*));
    
    for (int i = 0; i < 1000; i++) {
        lines[i] = calloc(100, sizeof(char));
    }

    // system("cd data/images/cloud;rm -f *.JPG;gphoto2 --get-all-files;ls *.JPG > ../../liste.txt;cd ../../..");
    FILE * FIC = fopen("data/liste.txt", "r");

    unsigned int x = 0;

    while (fgets(lines[x++], 99, FIC));

    char * commande = calloc(1000, sizeof(char));

    for (int i = 0; i < x; i++) {
        sprintf (commande, "exiv2 -px data/images/cloud/%s", lines[i]);
        printf ("%s\n", commande);
        system(commande);
    }

    for (int i = 0; i < 1000; i++) {
        free(lines[i]);
    }

    free(lines);
    free(commande);

    return 0;
}