#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_SUPPRESSIONS 1000
#define TAILLE_NOMS_IMAGES_MAX 150
#define NB_NOMS_MAX_BIG 1000
#define NB_NOMS_MAX_SMALL 1000

unsigned int len (char *chaine) {
    int taille = 0;

    while (chaine[taille] != 0 && !(chaine[taille] == ' ' && chaine[taille+1] == ' ')) {
        // S'il y a plus d'un espace, la boucle s'arrête
        taille++;
    }

    return taille;
}

void afficher_chaine_int (char *chaine) {
    int i = 0;

    while (chaine[i] != 0) {
        printf("%d", chaine[i]);
    }

    printf ("\n");
}

void extraire_noms (char text[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX], char tab[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	char dossier[TAILLE_NOMS_IMAGES_MAX] = "";

    char nombre_fichiers_dossier[8] = "";

    unsigned int nombre_fichier_dossier = 0;

	char nouvelleLigne[TAILLE_NOMS_IMAGES_MAX] = "";
	
	FILE* FICW = fopen("./camera-list-tmp.txt", "w");

	int i = 0, j, c = 0, x = 0, y = 0, esp = 0, file_number_analysed = 0, file_number;

	char trash_buffer[TAILLE_NOMS_IMAGES_MAX] = ""; // STOCKER LA FIN DE LA CHAINE DE CARACTÈRES

    int mx = 0, my = 0, dy = 0;

	while (strcmp(text[i], "") != 0) {
		// On executera ce bloc plus tard, pour la capture de dossier

		// for (j = strlen(text[i])-1; j >= 0; j--) {
		// 	if (text[i][j] == '«') {
		// 		break;
		// 	}

		// 	if (esp == 2) {
		// 		sscanf(trash_buffer, "%d", &file_number);
		// 	}

		// 	else if (esp < 2) {
		// 		trash_buffer[i] = text[i][j]; // ÉCRIS EN MIRROIR MAIS C'EST PAS GRAVE
		// 		i++;
		// 	}
		// }

        c = 0;
		if (text[i][0] == '#') {
			// C'est un fichier
			while (text[i][c] != ' ') {
				// On peut stoquer le numéro mais ca ne nous interesse pas
				c++;
			}

			while (text[i][c] == ' ') {
				c++;
			}

			while (text[i][c] != ' ') {
				// dossier[y] = text[i][c];
				tab[mx][my] = text[i][c];
                my++;
                c++;
				// y++;
			}

            fprintf (FICW, "%s\n", tab[mx]);

			mx++;

			y = 0;
		}

        else {
            // Capture du dossier si jamais des fichier y sont dedans mais on fera ca plus tard
            // Stratégie gauche droite
            c = len(text[i]);

            afficher_chaine_int(text[i]);

            printf ("strlen %d et len %d\n", strlen(text[i]), c);

            while (text[i][c] != ' ') {
                c--;
            }

            c--;

            while (text[i][c] != ' ') {
                // Verification de la présence d'un nombre en remplissant d'abord
                nombre_fichiers_dossier[dy] = text[i][c];
                dy++;
                c--;
            }

            dy = 0;

            if (sscanf(nombre_fichiers_dossier, "%d", &nombre_fichier_dossier) == 1) {
                printf ("Nombre de fichiers dans le dossier : %d\n", nombre_fichier_dossier);

                // C'est un dossier qui contient des fichiers, il faut donc cherches les fichiers

                while (text[i][c] != 175) {
                    // Placement
                    c--;
                    printf ("222\n");
                }

                c--;

                while (text[i][c] != ' ') {
                    // Remplissage
                    printf ("111\n");
                    dossier[dy] = text[i][c];
                    dy++;
                    c--;
                }
            }
        }

        my = 0;

		i++;
	}

	// for (i = 0; i < NB_NOMS_MAX_BIG; i++) {
	// 	if (tab[i][0] != 0) {
	// 		for (j = 0; j < TAILLE_NOMS_IMAGES_MAX; j++) {
	// 			sprintf (nouvelleLigne, "%c", tab[i][j]);
	// 		}

	// 		fprintf (FICW, "%s\n", nouvelleLigne);
	// 	}
	// }

	fclose(FICW);

    printf ("%s\n", dossier);

	// system("cat ./data/images/camera-list-tmp.txt > ./data/images/camera-list.txt");
	// system("rm -f ./data/images/camera-list-tmp.txt");
 
	FILE * FIC = fopen("./camera-list.txt", "r");

	i = 0;

	fclose(FIC);
}

int main () {
    FILE * fic = fopen("files_list.txt", "r");

    char text[1000][150];
    char names[1000][150];

    int i = 0, j;

    // Initialisations à 0

    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 149; j++) {
            text[i][j] = 0;
            names[i][j] = 0;
        }
    }

    i = 0;

    while (fgets(text[i], 149, fic)) {
        printf ("%s", text[i]);
        i++;
    }

    extraire_noms(text, names);

    fclose(fic);

    return 0;
}