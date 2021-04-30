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