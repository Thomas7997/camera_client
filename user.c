#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define INTERVALLE_DELAI 1

void lancer_commandes (void) {
	system("cd ./images/gets;gphoto2 --get-all-files;cd ../../");
	system("./transfert");
	
	// En fonction de la lecture de certains fichiers contenant des commandes, on execute le sync quand l'utilisateur le veut
	// system("sync");
}

int main () {
	while (1) {
		lancer_commandes();
		sleep(INTERVALLE_DELAI);
	}
	
	return 0;
}