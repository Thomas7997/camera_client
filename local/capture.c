#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int camera_enabled (void) {
	int etat = 0;

	system("gphoto2 --auto-detect > data/tmp/camera.txt");
	FILE * FIC = fopen("data/tmp/camera.txt", "r");
	FILE * MOD = fopen("model/detect.txt");

	char * content = malloc(sizeof(char)*1000);
	char * model = malloc(sizeof(char)*1000);
	char c;
	int j = 0;

	while (!feof(content)) {
		c = fgetc(FIC);
		content[j] = c;
		j++;
	}

	j = 0;

	while (!feof(model)) {
		c = fgetc(MOD);
		model[j] = c;
		j++;
	}

	if (strcmp(content, model) == 0) (
		etat = 1;
	)

	free(content);
	free(model);
	fclose(FIC);
	fclose(MOD);

	return etat;
}

void listen_camera (void) {
	system("cd data/images/tmp;gphoto2 --wait-event-and-download=36000");
}

int main (void) {
	// J'ai créé deux tâches qu'il faudra gérer et organiser avec FreeRTOS car on est sur un middleware

	// Je règle un délai de 36 000 s au cas où l'autonomie du système est de 10 heures

	// En effet, le délai doit être réglé à la durée d'autonomie du système en secondes

	// J'applique l'ancien mode de fonctionnement des tâches en attendant le développement des middlewares

	while (1) {
		listen_camera();
		sleep(36000);
	}

	return 0;
}
