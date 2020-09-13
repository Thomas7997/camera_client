#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display16 (uint8_t * hexa) {
    uint8_t number = 0;

    for (int i = 0; i < 4; i++) {
        printf ("%d ", hexa[i]);
    }

    printf ("\n");
}

void bf (uint8_t * x, uint8_t y) {
    display16(x);
    printf ("Requête URB\n");

    if (y == 4) {
        return;
    }

    for (int a = 0; a < 16; a++) {
        x[y] = a;
        bf(x, y+1);
    }
}

int main (void) {
    uint8_t * hexa = calloc(4, 8);

    bf(hexa, 0);

    free(hexa);
    hexa = NULL;
    return 0;
}