#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define prefix "IMG"
#define suffix ""

#define NOMS_MAX_SIZE 30
#define NB_NOMS 10
#define NB_REFERENCES 100

void init_tab (int chars, char tab[chars]) {
    int i;

    for (i = 0; i < chars; i++) {
        tab[i] = 0;
    }
}

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
        printf ("\n");
    }
}

void enlever_mise_ligne (char chaine[NOMS_MAX_SIZE-4]) {
	int i;

	for (i = 0; i < NOMS_MAX_SIZE-4; i++) {
		if (chaine[i] == 0) {
			chaine[i-1] = 0;
			break;
		}
	}
}

void get_extensions (char extensions[NB_NOMS][4], char noms[NB_NOMS][NOMS_MAX_SIZE]) {
    int i, j = 0, y = 0;

    for (i = 0; i < NB_NOMS; i++) {
        while (noms[i][j] != '.' && noms[i][j] != 0) {
            j++; // Trouver l'extrémité
        }

        j++;

        for (y = 0; y < 4; y++) {
            if (noms[i][j+y-1] != 'e' && y == 3) {
                break; // car on sais que ce n'est pas un jpeg
            }

            extensions[i][y] = noms[i][j+y];
        }

        j = 0;

        if (noms[i][0] == 0) {
            break;
        }
    }
}

int compter_0_nombre (int n) {
    if (n/10 > 0) {
        return compter_0_nombre(n/10) + 1;
    }

    else {
        return n/10;
    }
}

void copier_tab1 (int chars, char tab1[chars], char tab2[chars]) {
    int i;

    for (i = 0; i < chars; i++) {
        tab1[i] = tab2[i];
    }
}

void copier_tab1_int (int chars, int tab1[chars], int tab2[chars]) {
    int i;

    for (i = 0; i < chars; i++) {
        tab1[i] = tab2[i];
    }
}

void afficher_tab1 (int chars, char tab[chars]) {
    int i;

    for (i = 0; i < chars; i++) {
        printf ("%d ", tab[i]);
    }

    printf ("\n");

    for (i = 0; i < chars; i++) {
        printf ("%c ", tab[i]);
    }

    printf ("\n\n");
}

void inverser (int size, char chaine[size]) {
    int i;
    char c;

    for (i = 0; i < size/2; i++) {
        c = chaine[size-i-1];
        chaine[size-i-1] = chaine[i];
        chaine[i] = c;
    }
}

int char_to_int (int size, char *chaine) {
    int i = 0, nombre = 0; 

    for (i = 0; i < size; i++) {
        if (chaine[i] != 0) {
            printf("i : %d\nnombre : %d\n, chaine[%d] : %c\n", i, nombre, i, chaine[i]);
            nombre += (chaine[i] - '0') * pow(10, size - i - 1);
        }
    }

    printf ("nombre : %d\n", nombre);

    return nombre;
}

int affecter_multiples_10 (char nouveau[NOMS_MAX_SIZE-4], char dernier[NOMS_MAX_SIZE-4], int init) {
    int i = init, d = 0;

    while (dernier[i] == '9') {
        nouveau[i] = '0';
        i--;
        d++;
    }

    nouveau[d]++;

    return i;
}

// void affecter_nouvelle_reference (char nouveau[NOMS_MAX_SIZE-4], char dernier[NOMS_MAX_SIZE-4]) {
//     int nb_0, nombre = 0;

//     copier_tab1(NOMS_MAX_SIZE-4, nouveau, dernier);

//     nombre = char_to_int(NOMS_MAX_SIZE-4, nouveau);

//     printf ("NB : %d\n", nombre);

//     nb_0 = compter_0_nombre(nombre);

//     printf ("nb_0 : %d\n", nb_0);

//     int i = nb_0+1, d = 0;

//     while (i >= 0) {
//         if (dernier[i] == '9' && i == nb_0+1) {
//             i -= affecter_multiples_10(nouveau, dernier, i);
//         }
        
//         else if (dernier[i+d] != '0') {
//             nouveau[i] = dernier[i] + 1;
//             printf ("%c\n", dernier[i]);
//         }

//         else {
//             nouveau[i] = dernier[i];
//         }

//         i--;
//     }

//     afficher_tab1(i, nouveau);
//     enlever_mise_ligne (nouveau);
// }

void affecter_nouvelle_reference (char nouveau[NOMS_MAX_SIZE-4], char dernier[NOMS_MAX_SIZE-4]) {
    int nb_0, nombre = 0;

    nombre = char_to_int(NOMS_MAX_SIZE-4, dernier);
    printf ("NOMBRE : %d\n", char_to_int(NOMS_MAX_SIZE-4, dernier));
    nombre++;
    sprintf(nouveau, "%d", nombre);

    afficher_tab1(NOMS_MAX_SIZE-4, nouveau);
    enlever_mise_ligne (nouveau);
}

void convert_int2_to_char2 (int size, int tab1[size], char tab2[size]) {
    int i;

    for (i = 0; i < size; i++) {
        tab2[i] = tab1[i] - '0';
    }
}

int main () {
    char noms[NB_NOMS][NOMS_MAX_SIZE];
    char nouveaux_noms[NB_NOMS][NOMS_MAX_SIZE];
    char references[NB_REFERENCES][NOMS_MAX_SIZE-4];
    char nouvelle_reference[NOMS_MAX_SIZE-4] = "";
    char nouvelles_references[NB_REFERENCES][NOMS_MAX_SIZE-4];
    char extensions[NB_NOMS][4];
    int x1 = 0, x2 = 0;
    char commande[100] = "";

    init_tab2(NB_NOMS, NOMS_MAX_SIZE, noms);
    init_tab2(NB_NOMS, NOMS_MAX_SIZE, nouveaux_noms);
    init_tab2(NB_REFERENCES, NOMS_MAX_SIZE-4, references);
    init_tab(NOMS_MAX_SIZE-4, nouvelle_reference);
    init_tab2(NB_NOMS, 4, extensions);
    init_tab2(NB_REFERENCES, NOMS_MAX_SIZE-4, nouvelles_references);

    system ("ls ./image/cloud > ./image/liste.txt");

    FILE* TXT1 = fopen("./image/liste.txt", "r");
    FILE* TXT2 = fopen("./historique/references.txt", "r+");

    while (fgets(noms[x1], 40, TXT1) != NULL) {
		enlever_mise_ligne(noms[x1]);
        x1++;
        printf("%d\n", x1);
	}

    enlever_mise_ligne(noms[x1]);

    while (fgets(references[x2], 40, TXT2) != NULL) {
        enlever_mise_ligne(references[x2]);
        printf ("%s\n", references[x2]);
		x2++;
	}

    get_extensions(extensions, noms);

    int x = 0, y = 0, nb, cp;

    // Attribution des nouveaux noms

    for (x = 0; x < x1; x++) {
        affecter_nouvelle_reference (nouvelle_reference, references[x2-1]);
        // printf ("111\n%s\n111\n", extensions[x]);
        
        printf ("------ !\n");

        for (cp = 0; cp < NOMS_MAX_SIZE-4; cp++) {
            nouvelles_references[x][cp] = nouvelle_reference[cp];
            printf ("%c ", nouvelles_references[x][cp]);
        }

        sprintf (nouveaux_noms[x], "%s_%s%c%s", prefix, nouvelle_reference, '.', extensions[x]);
        printf ("%s\n", nouveaux_noms[x]);
    }

    FILE* TXT1W = fopen("./image/liste.txt", "w");
 
    nb = x;

    for (x = 0; x < x1; x++) {
        sprintf (commande, "mv ./image/cloud/%s ./image/cloud/%s", noms[x], nouveaux_noms[x]);

        printf ("-> %s\n", commande);

        system(commande);

        fprintf (TXT2, "%s\n", nouvelles_references[x]);
    }

    return 0;
}