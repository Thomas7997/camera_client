#include "scripts.h"

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

void clearList (char ** list) {
    unsigned int x = 0;

    for (char * str = *list; str != 0; str = *(list+x), x++) {
        strcpy(str, "");
    }
}

void clearStr (char * str) {
    for (unsigned int i = 0; str[i]; i++) {
        str[i] = 0;
    }
}

void operation_finished (const char * path, const char * name) {
    FILE * R = fopen(path, "r");
    FILE * W = fopen(path, "w");

    char ** names = (char**) calloc(MAX_DOWNLOADS, sizeof(char));

    for (int i = 0; i < MAX_DOWNLOADS; i++) {
        names[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    unsigned int x = 0, index, n;
    while (names[x], 99, R) {
        if (!strcmp(names[x], name)) index = x;
        x++;
    }

    n = x;

    for (x = 0; x < index; x++) {
        fprintf(W, "%s\n", names[x]);
    }

    for (x = index+1; x < n+1; x++) {
        fprintf(W, "%s\n", names[x]);
    }

    for (int i = 0; i < MAX_DOWNLOADS; i++) {
        free(names[i]);
        names[i] = NULL;
    }

    free(names);
    names = NULL;
    fclose(R);
    fclose(W);
}