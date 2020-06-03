#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NB_SUPPRESSIONS 1000
#define TAILLE_NOMS_IMAGES_MAX 30
#define NB_NOMS_MAX_BIG 1000
#define NB_NOMS_MAX_SMALL 1000

void init_tab2 (int lines, int chars, char tab[lines][chars]) {
	int i, j;

	for (i = 0; i < lines; i++) {
		for (j = 0; j < chars; j++) {
			tab[i][j] = 0;
		}
	}
}

void afficher_tab2 (int lines, int chars, char tab[lines][chars]) {
	int i, j;

	for (i = 0; i < lines; i++) {
		for (j = 0; j < chars; j++) {
			printf ("%c", tab[i][j]);
		}
	}
}

void extraire_noms (char text[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX], char tab[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	char dossier[TAILLE_NOMS_IMAGES_MAX] = "";

	char nouvelleLigne[TAILLE_NOMS_IMAGES_MAX] = "";
	
	FILE* FICW = fopen("./data/images/camera-list-tmp.txt", "w");

	int i, j, c = 0, x = 0, y = 0, esp = 0, file_number_analysed = 0, file_number;

	char trash_buffer[TAILLE_NOMS_IMAGES_MAX] = ""; // STOCKER LA FIN DE LA CHAINE DE CARACTÈRES

	// INIT TAB2
	for (i = 0; i < NB_NOMS_MAX_BIG; i++) {
		for (j = 0; j < TAILLE_NOMS_IMAGES_MAX; j++) {
			tab[i][j] = 0;
		}
	}

	while (strcmp(text[i], "") != 0) {
		printf("ligne : %s\n", text[i]);

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

		printf ("ligne[0] : %c\n", text[i][0]);
		if (text[i][0] == '#') {
			printf ("1\n");
			// C'est un fichier
			while (text[i][c] != ' ') {
				// On peut stoquer le numéro mais ca ne nous interesse pas
				c++;
			}

			while (text[i][c] == ' ') {
				c++;
			}

			while (text[i][c] != ' ') {
				dossier[y] = text[i][c];
				c++;
				y++;
			}

			x++;
			c = 0;

			y = 0;
		}

		i++;
	}

	for (i = 0; i < NB_NOMS_MAX_BIG; i++) {
		if (tab[i][0] != 0) {
			for (j = 0; j < TAILLE_NOMS_IMAGES_MAX; j++) {
				sprintf (nouvelleLigne, "%c", tab[i][j]);
			}

			fprintf (FICW, "%s\n", nouvelleLigne);
		}
	}	

	// system("cat ./data/images/camera-list-tmp.txt > ./data/images/camera-list.txt");
	// system("rm -f ./data/images/camera-list-tmp.txt");
 
	FILE * FIC = fopen("./data/images/camera-list.txt", "r");

	i = 0;

	// Actions ici

	fclose(FIC);
}

void enlever_mise_ligne (char chaine[TAILLE_NOMS_IMAGES_MAX]) {
	int i;

	for (i = 0; i < TAILLE_NOMS_IMAGES_MAX; i++) {
		if (chaine[i] == 0) {
			chaine[i-1] = 0;
			break;
		}
	}
}

// Voir si la nouvelle photo existe dans le cloud
void comparer_liste_images_f_txt (FILE* TXT3, int *envois, char noms[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX], char noms3[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX]) {
	int x2 = 0, x3 = 0, size2, size3, trouve;

	init_tab2(NB_NOMS_MAX_SMALL, TAILLE_NOMS_IMAGES_MAX, noms3);

	// Remplir la grande et la petite liste

	size2 = x2;
	x2 = 0;

	while (fgets(noms3[x3], 30, TXT3) != NULL) {
		enlever_mise_ligne(noms3[x3]);
		x3++;
	}

	size3 = x3;
	x3 = 0;

	// A finir

	int x_envois = 0, x_supp = 0;

	// Comparer la liste

	for (x2 = 0; x2 < size2; x2++) {
		trouve = 0;

		for (x3 = 0; x3 < size3; x3++) {
			if (strcmp(noms3[x3], noms[x2]) == 0) {
				trouve = 1;
			}
		}

		if (trouve != 1) {
			envois[x_envois] = x2;
			printf ("%d ", x2);
			x_envois++;
		}
	}
}

// Transférer les nouvelles photos dans l'espace pour la capture

void envoyer_lignes (int envois[NB_SUPPRESSIONS], char noms[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	int i = 0;
	char commande[100] = "";

	while (envois[i] != NB_NOMS_MAX_SMALL + 1) {
		// Copier vers images/cloud et supprimer dans image/cloud

		printf("TRANSFERT\n");

		sprintf(commande, "cd ./image/cloud;gphoto2 --get-file=\"/store_00020001/DCIM/100EOS5D/%s\";cd ../..", noms[envois[i]]);

		printf("--> %s\n", commande);

		// Commande pour le transfert de la capture
		system(commande);

		i++;
	}
}

int main (void) {
	char noms[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX];
	char noms3[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX];

	char text_analyse[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX];

    int envois[NB_SUPPRESSIONS];
	int suppressions[NB_SUPPRESSIONS]; // Stoque les indices des éléments à supprimer (lignes)
	int suppressions3[NB_SUPPRESSIONS];

	int i;

	char image[31];

	system("ls ./data/image/cloud > ./data/image/liste.txt");
	system("gphoto2 --list-files > ./data/images/camera-list.txt");

	FILE* TXT3;
	TXT3 = fopen("./data/image/liste.txt", "r");
	FILE* LISTE;
	LISTE = fopen("./data/images/camera-list.txt", "r");

	// Il faudra créer un autre fichier avec la liste des historiques et son tableau de noms correspondant
	/* ici ... */

	for (i = 0; i < NB_SUPPRESSIONS; i++) {
		envois[i] = NB_NOMS_MAX_SMALL+1;
	}

	i = 0;

	while (fgets(text_analyse[i], TAILLE_NOMS_IMAGES_MAX, LISTE) != NULL) {
		i++; // Remplir le tableau text_analyse
	}

	extraire_noms(text_analyse, noms);

	afficher_tab2(NB_NOMS_MAX_BIG, TAILLE_NOMS_IMAGES_MAX, noms);

	comparer_liste_images_f_txt(TXT3, envois, noms, noms3);
	envoyer_lignes(envois, noms);

	system("ls ./data/image/cloud > ./data/image/liste.txt");
	printf("EXECUTION TRANSFERT FAIT\n");

	return 0;
}
