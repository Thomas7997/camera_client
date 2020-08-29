#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 80 

#define MAX_CAPTURES 10000
#define TAILLE_NOM 30

void mirroir (char *chaine);

void vider_buffer (char *buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = 0;
    }
}

void afficher_tab2 (char tab[MAX_CAPTURES][TAILLE_NOM]) {
    for (int i = 0; i < MAX_CAPTURES; i++) {
        for (int j = 0; j < TAILLE_NOM; j++) {
            printf ("%c", tab[i][j]);
        }
        printf ("\n");
    }
}

// Il faut enlever l'extension
void transform_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char nouvelleListe[MAX_CAPTURES][TAILLE_NOM], int size) {
    int i = 0, j;

    for (i = 0; i < size; i++) {
        j = 0;
        while (liste[i][j] != '.') {
            nouvelleListe[i][j] = liste[i][j];
            printf ("%c", nouvelleListe[i][j]);
            j++;
        }
        printf ("\n");
    }
}

void transferer_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char old[MAX_CAPTURES][TAILLE_NOM]) {
    int i = 0, j;

    char commande[250] = "";
    char path[50] = "";
    printf ("1\n");

    while (liste[i][0] != 0) {
        sprintf(commande, "base64 ./data/images/tmp/%s > ./data/datas/tmp/%s.txt", old[i], liste[i]);
        // sprintf(commande, "mv ./data/capture/%s ./data/capture/cloud/%s;mv ./data/capture/cloud/%s ./data/images/cloud", old[i], liste[i], liste[i]);
    	// system(commande);
        // Envoyer le nom du nouveau du nouveau fichier transféré au socket
        system(commande);
        
        printf ("2\n");

        sprintf(path, "./data/datas/tmp/%s.txt", liste[i]);

        FILE * FIC = fopen(path, "r");
        char * base64 = malloc(sizeof(char)*10000000);

        printf ("3\n");

        j = 0;

        char c;

        if (FIC == NULL) {
            printf ("FICHIER INCORRECTE\n%s\n", path);
            exit(1);
        }

        while (!feof(FIC)) {
            // Remplissage
            c = fgetc(FIC);
            if (c != '\n') {
                base64[j] = c;
            }
            // printf("%c", base64[j]);
            j++;
        }

        base64[j-2] = 0;

        free(FIC);

        printf ("4\n");

        char * envoi = malloc(sizeof(char)*10000000);

        strcpy(envoi, "");

        sprintf(envoi, "curl -d '{\"data\":\"data:image/jpeg;base64,%s\"}' -H \"Content-Type: application/json\" -X POST http://localhost:8000/transfert/photo\nrm -f ./data/datas/tmp/%s.txt ./data/images/tmp/%s", base64, liste[i], old[i]);

        // Peut etre vider le buffer avant avec une fonction créée.

        FILE * SCRIPT = fopen("data/tmp/script.sh", "w");
        fprintf(SCRIPT, "%s", envoi);

        system(envoi);
        system("chmod +x data/tmp/script.sh");
        system("bash data/tmp/script.sh");
        fclose(FIC);
        free(envoi);
        fclose(SCRIPT);
        
        i++;
    }
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

int main (void) {
    system("cd data/images/tmp;ls capt* > ../liste.txt;cd ../../..");

    FILE * CAPTURES = fopen("./data/images/liste.txt", "r");

    printf ("1\n");

    char liste_captures[MAX_CAPTURES][TAILLE_NOM];
    char nouvelles_captures[MAX_CAPTURES][TAILLE_NOM];

    int i, j, number = 0;

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            liste_captures[i][j] = 0;
            nouvelles_captures[i][j] = 0;
	    }
    }

    i = 0;

    while (fgets(liste_captures[i], TAILLE_NOM, CAPTURES) != NULL) {
        // Remplissage
        enlever_last_car(liste_captures[i++]);
        number++;
    }

    if (strcmp(liste_captures[i], "ls: cannot access 'cloud/capt*': No such file or directory") == 0) {
	    return 1;
    }

    i = 0;

    printf ("1\n");

    transform_noms(liste_captures, nouvelles_captures, number);

    printf("%d\n", number);

    transferer_noms(nouvelles_captures, liste_captures);

    fclose(CAPTURES);

    system("ls ./data/images/tmp > ./data/images/liste.txt");

    return 0;
}
