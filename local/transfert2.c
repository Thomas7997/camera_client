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
			printf ("%d", tab[i][j]);
		}
	}
}

void extraire_noms (FILE* FIC) {
	char noms[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX];
	char ligne[TAILLE_NOMS_IMAGES_MAX] = "";
	char nouvelleLigne[TAILLE_NOMS_IMAGES_MAX] = "";
	
	FILE* FICW = fopen("./data/images/camera-list-tmp.txt", "w");

	int i, j, c = 0, x = 0, y = 0;

	// INIT TAB2
	for (i = 0; i < NB_NOMS_MAX_BIG; i++) {
		for (j = 0; j < TAILLE_NOMS_IMAGES_MAX; j++) {
			noms[i][j] = 0;
		}
	}

	while (fgets(ligne, 30, FIC) != NULL) {
		if (ligne[0] == '#') {
			// C'est un fichier
			while (ligne[c] != ' ') {
				// On peut stoquer le numéro mais ca ne nous interesse pas
				c++;
			}

			while (ligne[c] == ' ') {
				c++;
			}

			while (ligne[c] != ' ') {
				noms[x][y] = ligne[c];
				c++;
				y++;
			}

			x++;
			c = 0;

			y = 0;
		}
	}

	for (i = 0; i < NB_NOMS_MAX_BIG; i++) {
		if (noms[i][0] != 0) {
			for (j = 0; j < TAILLE_NOMS_IMAGES_MAX; j++) {
				sprintf (nouvelleLigne, "%c", noms[i][j]);
			}

			fprintf (FICW, "%s\n", nouvelleLigne);
		}
	}	

	system("cat ./data/images/camera-list-tmp.txt > ./data/images/camera-list.txt");
	system("rm -f ./data/images/camera-list-tmp.txt");
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
void comparer_liste_images_f_txt (FILE* TXT, FILE* TXT3, int *suppressions, int *suppressions3, int *envois, char noms2[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX], char noms3[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX]) {
	int x2 = 0, x3 = 0, size2, size3, trouve;

	init_tab2(NB_NOMS_MAX_SMALL, TAILLE_NOMS_IMAGES_MAX, noms2);

	// Remplir la grande et la petite liste

	while (fgets(noms2[x2], 30, TXT) != NULL) {
		enlever_mise_ligne(noms2[x2]);
		x2++;
	}

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

	// Comparer la grande et la petite liste

	for (x2 = 0; x2 < size2; x2++) {
		trouve = 0;

		for (x3 = 0; x3 < size3; x3++) {
			if (strcmp(noms3[x3], noms2[x2]) == 0) {
				trouve = 1;
			}
		}

		if (trouve == 1) {
			suppressions[x_supp] = x2;
			x_supp++;
		}

		else {
			envois[x_envois] = x2;
			x_envois++;
		}
	}
}

// Transférer les nouvelles photo dans l'espace pour la capture

void envoyer_lignes (int envois[NB_SUPPRESSIONS], char noms[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	int i = 0;
	char commande[100] = "";

	while (envois[i] != NB_NOMS_MAX_SMALL + 1) {
		// Copier vers images/cloud et supprimer dans image/cloud

		printf("TRANSFERT\n");

		sprintf(commande, "cd ./image/cloud;gphoto2 --get-file=s\"%s\";cd ../..", noms[envois[i]]);

		printf("--> %s\n", commande);

		// Commande pour le transfert de la capture
		system(commande);

		i++;
	}
}

void supprimer_lignes (int suppressions[NB_SUPPRESSIONS], int suppressions3[NB_SUPPRESSIONS], char noms1[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	int i = 0;
	char commande[100] = "";

	while (suppressions[i] != NB_NOMS_MAX_SMALL + 1) {
		// Copier vers images/cloud et supprimer dans image/cloud

		printf("SUPPRESSION\n");

		sprintf(commande, "rm -f \"./images/gets/%s\"", noms1[suppressions[i]]);

		printf("--> %s\n", commande);

        system(commande);

		i++;
	}

	i = 0;
}

int main (void) {
	char noms2[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX];
	char noms3[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX];

    int envois[NB_SUPPRESSIONS];
	int suppressions[NB_SUPPRESSIONS]; // Stoque les indices des éléments à supprimer (lignes)
	int suppressions3[NB_SUPPRESSIONS];

	int i;

	char image[31];

	system("ls ./data/image/cloud > ./data/image/liste.txt");
	system("gphoto2 --list-files > ./data/images/camera-list.txt");

	FILE* TXT3 = fopen("./data/image/liste.txt", "r");
	FILE* LISTE = fopen("./data/images/camera-list.txt", "r");

	// Il faudra créer un autre fichier avec la liste des historiques et son tableau de noms correspondant
	/* ici ... */

	for (i = 0; i < NB_SUPPRESSIONS; i++) {
		envois[i] = NB_NOMS_MAX_SMALL+1;
	}

	extraire_noms(LISTE);

	comparer_liste_images_f_txt(LISTE, TXT3, suppressions, suppressions3, envois, noms2, noms3);
	envoyer_lignes(envois, noms2);

	system("ls ./data/image/cloud > ./data/image/liste.txt");
	printf("EXECUTION TRANSFERT FAIT\n");

	return 0;
}
