#include "transferts.h"

int main (void) {
    unsigned int size_historique;
    unsigned int size_transferts;
    char ** historique = getHistorique(&size_historique);
    char ** transferts = getTransferts(&size_transferts);

    printf("%d\n%d\n\n", size_historique, size_transferts);

    /*

        Il faut gérer les listes de fichiers pour permettre un transfert uniquement lorsque
        un fichier n'a pas été transféré avec le moins de complexité que possible.
        Le but est aussi de transferer dès qu'un nouveau nom apparaît dans la liste de noms de fichiers (transferts.txt)

        Objectifs : 1) Détecter l'apparition d'un nom
                    2) Comparer avec l'historique
                    3) Commander un transfert (sans transférer)
        
        Compilation : g++ -o transferts transferts.cpp main.cpp

    */

    freeHistorique(historique);
    freeTransferts(transferts);
    return 0;
}