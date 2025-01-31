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
void comparer_liste_images_f_txt (FILE* TXT, FILE* TXT2, int *suppressions, int *envois, char noms1[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX], char noms2[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX]) {
	int x1 = 0, x2 = 0, size1, size2, nb_differences = 0;

	init_tab2(100, TAILLE_NOMS_IMAGES_MAX, noms1);
	init_tab2(100, TAILLE_NOMS_IMAGES_MAX, noms2);

	// Remplir la grande et la petite liste

	while (fgets(noms1[x1], 30, TXT) != NULL) {
		enlever_mise_ligne(noms1[x1]);
		x1++;
	}

	size1 = x1;
	x1 = 0;

	while (fgets(noms2[x2], 30, TXT2) != NULL) {
		enlever_mise_ligne(noms2[x2]);
		x2++;
	}

	size2 = x2;
	x2 = 0;

	int x_envois = 0, x_supp = 0;

	// Comparer la grande et la petite liste

	for (x2 = 0; x2 < size2; x2++) {
		for (x1 = 0; x1 < size1; x1++) {
			if (strcmp(noms1[x1], noms2[x2]) != 0) {
				nb_differences++;
			}

			else {
				suppressions[x_supp] = x2; // Tableau qui s'applique à la petite liste
				x_supp++;
			}
		}

		if (nb_differences == x1) {
			envois[x_envois] = x2; // Indice de ligne
			x_envois++;
			nb_differences = 0;
		}

	}
}

// Transférer la nouvelle photo dans le cloud

void envoyer_lignes (int envois[NB_SUPPRESSIONS], char noms[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	int i = 0;
	char commande[100] = "";

	while (envois[i] != NB_NOMS_MAX_SMALL + 1) {
		// Copier vers images/cloud et supprimer dans image/cloud

		printf("TRANSFERT\n");

		sprintf(commande, "cp \"./image/cloud/%s\" ./images/cloud", noms[envois[i]]);

		printf("--> %s\n", commande);

		// Commande pour le transfert en sync
		system(commande);

		// Commande pour supprimer ensuite

		sprintf(commande, "rm \"image/cloud/%s\"", noms[envois[i]]);

		printf("--> %s\n", commande);

		system(commande);

		i++;
	}
}

void supprimer_lignes (int suppressions[NB_SUPPRESSIONS], char noms[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX]) {
	int i = 0;
	char commande[100] = "";

	while (suppressions[i] != NB_NOMS_MAX_SMALL + 1) {
		// Copier vers images/cloud et supprimer dans image/cloud

		printf("SUPPRESSION\n");

		sprintf(commande, "rm -f \"./image/cloud/%s\"", noms[suppressions[i]]);

		printf("--> %s\n", commande);

    	system(commande);

		i++;
	}
}

int main (int argc, char **argv) {
	char noms1[NB_NOMS_MAX_BIG][TAILLE_NOMS_IMAGES_MAX];
	char noms2[NB_NOMS_MAX_SMALL][TAILLE_NOMS_IMAGES_MAX];

  	int envois[NB_SUPPRESSIONS];
	int suppressions[NB_SUPPRESSIONS]; // Stoque les indices des éléments à supprimer (lignes)

	int i;

	FILE* TXT1 = fopen("./data/images/liste.txt", "r");

	FILE* TXT2 = fopen("./data/image/liste.txt", "r");


	if (strcmp(argv[1], "mv") == 0) {
		system("mv ./data/images/cloud/* ./data/image/cloud");
		system("ls ./data/image/cloud > ./data/image/liste.txt");
		system("ls ./data/images/cloud > ./data/images/liste.txt");

		return 0;
	}

	char image[31];
	system("ls ./data/images/cloud > ./data/images/liste.txt");
	system("ls ./data/image/cloud > ./data/image/liste.txt");
	TXT1 = fopen("./data/images/liste.txt", "r");
	TXT2 = fopen("./data/image/liste.txt", "r");

	for (i = 0; i < NB_SUPPRESSIONS; i++) {
		envois[i] = NB_NOMS_MAX_SMALL+1;
		suppressions[i] = NB_NOMS_MAX_SMALL+1;
	}

	comparer_liste_images_f_txt(TXT1, TXT2, suppressions, envois, noms1, noms2);
	envoyer_lignes(envois, noms2);
    	supprimer_lignes(suppressions, noms2);

	system("ls ./data/images/cloud > ./data/images/liste.txt");
        system("ls ./data/image/cloud > ./data/image/liste.txt");

	printf ("EXECUTION\n");

	
	
	return 0;
}
