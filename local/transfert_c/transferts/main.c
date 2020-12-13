#include "transferts.h"

int main (void) {
    unsigned int size_historique;
    unsigned int size_transferts;
    char ** c_transferts = (char**) calloc(10000, sizeof(char*));
    
    for (unsigned int e = 0; e < 10000; e++) {
        c_transferts[e] = (char*) calloc(100, sizeof(char));
    }

    char ** historique;
    char ** transferts;

    while (1) {
        historique = (char**) getHistorique(&size_historique);
        transferts = (char**) getTransferts(&size_transferts);
        afficherList(historique, size_historique);
        afficherList(transferts, size_transferts);
        commande_transfert(historique, transferts, &size_transferts, &size_historique, c_transferts);
        usleep(5000);
    }
    
    /*

        Il faut gérer les listes de fichiers pour permettre un transfert uniquement lorsque       /home/mina/Bureau/transferts
        un fichier n'a pas été transféré avec le moins de complexité que possible.
        Le but est aussi de transferer dès qu'un nouveau nom apparaît dans la liste de noms de fichiers (transferts.txt)

        Objectifs : 1) Détecter l'apparition d'un nom
                    2) Comparer avec l'historique
                    3) Commander un transfert (sans transférer)
        
        Compilation : g++ -o transferts transferts.cpp main.cpp

    */

    for (unsigned int e = 0; e < 10000; e++) {
        free(c_transferts[e]);
    }

    free(c_transferts);
    freeHistorique(historique);
    freeTransferts(transferts);
    return 0;
}