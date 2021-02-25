#include<stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct __LISTE {
    char nom[100];
    struct __LISTE * suiv;
} LISTE;

int main (void) {
    LISTE * liste = malloc(sizeof(LISTE));
    strcpy(liste->nom, "Thomas");
    printf("%s\n", liste->nom);

    return 0;
}