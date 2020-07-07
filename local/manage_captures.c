#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPTURES 10000
#define TAILLE_NOM 15

int last_num (char liste[MAX_CAPTURES][TAILLE_NOM]) {
    int nombre, i, j, tmp_nb;

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            sscanf(liste[i], "%d", &tmp_nb);
            if (tmp_nb > nombre) {
                nombre = tmp_nb;
            }
        }
    }

    return nombre; // Nombre maximum
}

void transform_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char autres[MAX_CAPTURES][TAILLE_NOM]) {
    int i, j, x;
    char ext[5] = "";
    int nombre_new, nombre_ex, nombre;

    char nouvelleListe[MAX_CAPTURES][TAILLE_NOM];

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            nouvelleListe[i][j] = 0;
        }
    }

    for (i = 0; i < TAILLE_NOM; i++) {
        x = 0;
        j = strlen(liste[i]) - 1;

        while (liste[i][j] != '.') {
            ext[x++] = liste[i][j--];
        }

        sscanf(liste[i], "%d", &nombre_ex);

        nombre = nombre_ex + last_num(autres);

        sprintf(liste[i], "IMG_%d.%s", nombre, ext);

        // A confirmer en testant
    }
}

void transferer_noms (char liste[MAX_CAPTURES][TAILLE_NOM]) {
    int i = 0;

    char commande[100] = "";

    while (strcmp(liste[i], "") != 0) {
        sprintf (commande, "mv ./data/capture/cloud/%s ./data/images/cloud", liste[i++]);
    }
}

int main (void) {
    FILE * CAPTURES = fopen("./data/capture/liste.txt", "r");
    FILE * PHOTOS = fopen("./data/images/liste.txt", "r");

    char liste_captures[MAX_CAPTURES][TAILLE_NOM];
    char liste_anciens[MAX_CAPTURES][TAILLE_NOM];

    int i, j;

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            liste_captures[i][j] = 0;
            liste_anciens[i][j] = 0;
        }
    }

    i = 0;

    while (fgets(liste_captures[i++], TAILLE_NOM, CAPTURES) != NULL) {
        // Remplissage
        printf("%d\n", i);
    }

    i = 0;

    while (fgets(liste_anciens[i++], TAILLE_NOM, PHOTOS) != NULL) {
        // Remplissage
        printf ("%d\n", i);
    }

    transform_noms(liste_captures, liste_anciens);

    transferer_noms(liste_captures);

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            printf ("%c", liste_captures[i][j]);
        }
        printf ("\n");
    }

    fclose(CAPTURES);

    system("ls ./data/images/cloud > ./data/images/liste.txt");
    system("ls ./data/capture/cloud > ./data/capture/liste.txt");

    return 0;
}