#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPTURES 10000
#define TAILLE_NOM 30

void mirroir (char *chaine);

void vider_buffer (char *buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = 0;
    }
}

int last_num (char liste[MAX_CAPTURES][TAILLE_NOM]) {
    int nombre = 0, i = 0, j, tmp_nb = 0, x;
    char nombre_str[10] = "";

    while (strcmp(liste[i], "") != 0) {
	    vider_buffer(nombre_str);

	    x = 0;
	
	   // Pour se situer

            j = strlen(liste[i]) - 1;
            while (liste[i][j] != '.') {
                j--;
	    }

	    j--;

	    while (liste[i][j] != '_') {
                nombre_str[x++] = liste[i][j--];
            }

	    i++;


            mirroir (nombre_str);

            sscanf(nombre_str, "%d", &tmp_nb);

	    if (tmp_nb > nombre) {
                nombre = tmp_nb;
            }
    }

    return nombre; // Nombre maximum
}

void mirroir (char *chaine) {
    int i;
    char car;

    for (i = 0; i < strlen(chaine) / 2; i++) {
        car = chaine[strlen(chaine) - i - 1];
        chaine[strlen(chaine) - i - 1] = chaine[i];
        chaine[i] = car;
    }
}

void transform_noms (char liste[MAX_CAPTURES][TAILLE_NOM], int nombre_new, char nouvelleListe[MAX_CAPTURES][TAILLE_NOM]) {
    int i = 0, j, x;
    char ext[5] = "";
    int nombre_ex, nombre;

    printf("nombre_new : %d\n", nombre_new);

    char nombre_str[10] = "";

    while (strcmp(liste[i],"") != 0) {
        x = 0;
        j = strlen(liste[i]) - 1;

        while (liste[i][j] != '.') {
            ext[x++] = liste[i][j--];
        }

        j--;
        x = 0;
	vider_buffer(nombre_str);

	while (liste[i][j] != 't') {
	    nombre_str[x++] = liste[i][j--];
        }

        mirroir(ext);       
	mirroir(nombre_str); 

	nombre_ex = atoi(nombre_str);
        nombre = nombre_ex + nombre_new;

        sprintf(nouvelleListe[i], "IMG_%d.%s", nombre, ext);

	printf ("FIC : %s\n", nouvelleListe[i]);
	
        i++;
	
	// A confirmer en testant
    
    }
}

void transferer_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char old[MAX_CAPTURES][TAILLE_NOM]) {
    int i = 0;

    char commande[100] = "";

    while (strcmp(liste[i], "") != 0) {
        sprintf (commande, "mv ./data/capture/%s ./data/capture/cloud/%s;mv ./data/capture/cloud/%s ./data/images/cloud", old[i], liste[i], liste[i++]);
    	system(commande);
    }
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

int main (void) {
    system("cd data/capture;ls cloud/capt* > liste.txt;cd ../..");
    system("cd data/images;ls cloud > liste.txt;cd ../..");

    FILE * CAPTURES = fopen("./data/capture/liste.txt", "r");
    FILE * PHOTOS = fopen("./data/images/liste.txt", "r");

    char liste_captures[MAX_CAPTURES][TAILLE_NOM];
    char liste_anciens[MAX_CAPTURES][TAILLE_NOM];
    char nouvelles_captures[MAX_CAPTURES][TAILLE_NOM];

    int i, j;

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            liste_captures[i][j] = 0;
            liste_anciens[i][j] = 0;
            nouvelles_captures[i][j] = 0;
	}
    }

    i = 0;

    while (fgets(liste_captures[i], TAILLE_NOM, CAPTURES) != NULL) {
        // Remplissage
        enlever_last_car(liste_captures[i++]);
    }

    if (strcmp(liste_captures[i],"ls: cannot access 'cloud/capt*': No such file or directory") == 0) {
	return 1;
    }

    i = 0;

    while (fgets(liste_anciens[i], TAILLE_NOM, PHOTOS) != NULL) {
        // Remplissage
        enlever_last_car(liste_anciens[i++]);
    }

    transform_noms(liste_captures, last_num(liste_anciens), nouvelles_captures);

    transferer_noms(nouvelles_captures, liste_captures);

    fclose(CAPTURES);
    fclose(PHOTOS);

    system("ls ./data/images/cloud > ./data/images/liste.txt");
    system("ls ./data/capture/cloud > ./data/capture/liste.txt");

    return 0;
}
