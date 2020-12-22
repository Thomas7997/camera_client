#include "transactions.h"

unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; y < nb_files[x]; y++) {
            sprintf(list[item++], "%s/%s", dirs[x], dossiers[x][y]);
        }
        
        x++;
    }

    return item;
}