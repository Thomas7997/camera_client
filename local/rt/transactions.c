#include "transactions.h"

unsigned int dossiers_to_list (char *** dossiers, char ** list, unsigned int nb_dossiers) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; dossiers[x][y][0] != 0; y++) {
            sprintf(list[item++], "%s", dossiers[x][y]);
        }
        
        x++;
    }

    return item;
}