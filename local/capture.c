#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int camera_enabled (void) {
	int etat = 0;

	system("gphoto2 --auto-detect > data/tmp/camera.txt");
	FILE * FIC = fopen("data/tmp/camera.txt", "r");
	FILE * MOD = fopen("model/detect.txt", "r");

	char * content = calloc(1000, sizeof(char));
	char * model = calloc(1000, sizeof(char));
	char c;
	int j = 0;

	while (!feof(FIC)) {
		c = fgetc(FIC);
		content[j] = c;
		j++;
	}

	j = 0;

	while (!feof(MOD)) {
		c = fgetc(MOD);
		model[j] = c;
		j++;
	}

	if (strcmp(content, model) != 0) {
		etat = 1;
	}

	free(content);
	free(model);
	fclose(FIC);
	fclose(MOD);

	return etat;
}

void listen_camera (void) {
	system("cd data/images/tmp;gphoto2 --wait-event-and-download=36000s");
}

int main (void) {
	// J'ai créé deux tâches qu'il faudra gérer et organiser avec FreeRTOS car on est sur un middleware

	// Je règle un délai de 36 000 s au cas où l'autonomie du système est de 10 heures

	// En effet, le délai doit être réglé à la durée d'autonomie du système en secondes

	// J'applique l'ancien mode de fonctionnement des tâches en attendant le développement des middlewares

	while (1) {
		if (camera_enabled() == 1) {
			// Activer LED de succès
			// Envoyer un signal à l'application

			while (1) {
				listen_camera();
				sleep(36000);
			}

			break;
		}

		else {
			// Activer LED d'erreur
			// Envoyer un signal à l'application
			printf ("En attente de l'activation de la caméra.\n");
		}
	}

	return 0;
}
