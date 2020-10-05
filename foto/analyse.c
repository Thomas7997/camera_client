#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int nb = 2; // 2, 3, 4, 5, ... 10

// Algo de détection de répétitions

void addcat (unsigned char * word, unsigned char * list, unsigned int start) {
    int i, x = 0;

    for (i = start; i <= start+nb; i++) {
        word[x++] = list[i];
    }
}

int compare_each (unsigned char * list, unsigned char val, unsigned int nb) {
    int i;

    for (i = 0; i < nb; i++) {
        if (list[i] != val) {
            return 0;
        }
    }

    return 1;
}

unsigned int compare (unsigned char ** keys, unsigned char * img1, unsigned char * img2, unsigned int size, unsigned int x) {
    unsigned int x1 = 0, x2 = 0, c = 0, y = 0;
    unsigned char * word = calloc(nb+1, sizeof(unsigned char));
    
    if (img1[x] == 0) {
        free(word);
        return 0;
    }

    else {
        for (x1 = nb*x; x1 <= nb*(x+1); x1++) {
            // Élément à comparer
            strcpy(word, "");
            addcat(word, img1, x1);

            // Parcourir tab2
            for (x2 = 0; x2 < size; x2++) {
                if (compare_each(word,img2[x2],x2+1)) {
                    break;
                }

                else if (x2 % nb != 0) {
                    keys[x+y][c++] = img2[x2];
                }

                else {
                    // Passer à la clé suivant après sauvegarder
                    y++;
                }
            }
        }

        free(word);
        return compare(keys, img1, img2, size, x+1);
    }
}

unsigned int get (char * line, char * list, unsigned int size) {
    unsigned int i, x = 0;

    for (i = 0; i < size; i++) {
        if (line[i] != ' ') {
            list[x] = line[i];
            x++;
        }
    }

    return x+1;
}

void afficher_valeurs (unsigned char * list, unsigned int size) {
    int i;

    for (i = 0; i < size; i++) {
        printf ("%c\n", list[i]);
    }
}

int main (void) {
    FILE * IMG1 = fopen("img1.txt", "r");
    FILE * IMG2 = fopen("img2.txt", "r");
    FILE * IMG3 = fopen("img3.txt", "r");

    unsigned char * img1 = calloc(1500000, sizeof(unsigned char));
    unsigned char * img2 = calloc(1500000, sizeof(unsigned char));
    unsigned char * img3 = calloc(1500000, sizeof(unsigned char));
    unsigned char * line = calloc(1500000, sizeof(unsigned char));
    unsigned char ** keys = calloc(100, sizeof(unsigned char*));
    unsigned char * word = calloc(10, sizeof(unsigned char));

    for (int i = 0; i < 100; i++) {
        keys[i] = calloc(nb+1, sizeof(unsigned char));
    }

    unsigned int i = 0;
    char c = 0;

    while (!feof) {
        c = fgetc(IMG1);
        if (c != ' ') {
            line[i] = c;
            printf ("%c ", line[i++]);
        }
    }
    printf ("\n");

    printf("%s\n", line);
    unsigned int s1 = get(img1, line, strlen(line));
    printf("1\n");
    fgets(line, 1500000, IMG2);
    unsigned int s2 = get(img2, line, strlen(line));
    fgets(line, 1500000, IMG3);
    unsigned int s3 = get(img3, line, strlen(line));

    afficher_valeurs(img1, s1);

    int y = 0;

    compare(keys, img1, img2, s2, 0);
    // Sauvegarder les clés

    for (int i = 0;  i < 100; i++) {
        free(keys[i]);
    }

    free(img1);
    free(img2);
    free(img3);
    free(keys);
    free(word);
    free(line);

    fclose(IMG1);
    fclose(IMG2);
    fclose(IMG3);
}