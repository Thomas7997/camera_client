#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define INTERVALLE_DELAI 50000

void lancer_commandes (void) {
	// system("cd ./data/images/gets;gphoto2 --get-all-files;cd ../../..");
	printf ("Transmission\n");
	system("./transfert2");
	
	// En fonction de la lecture de certains fichiers contenant des commandes, on execute le sync quand l'utilisateur le veut
	// system("sync");
}

void verifierCommandeCapture (void) {
	system("cat ./data/image/capture.txt > ./data/image/capture_tmp.txt");

	FILE* FIC = fopen("./data/image/capture_tmp.txt", "r");
	FILE* FICW = fopen("./data/image/capture.txt", "w");

	int commande = 0;

	fscanf(FIC, "%d", &commande);

	if (commande == 1) {
		// system("cd ./data/image/capture;gphoto2 --capture-image;cd ../../..");
		printf ("Capture !!!\n");

		fprintf(FICW, "0"); // Désactiver la commande après son utilisation
	}

	system("rm -f ./data/image/capture_tmp.txt");

	fclose(FIC);
}

int main () {
	while (1) {
		lancer_commandes();

		// Verifier la commande de capture pour les captures à distance plus tard

		usleep(INTERVALLE_DELAI);
	}
	
	return 0;
}
