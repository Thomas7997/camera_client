#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <curl/curl.h>

int find (char ** lines, char * model, unsigned int nb_lines) {
	int y = 9, i = 0, j = 0;
	int etat = 0;

	char * line = calloc(1000, sizeof(char));

	for (i = 0; i < nb_lines; i++) {
		while (lines[i][y] != 0) {
			// On change de ligne

			line[j] = lines[i][y];
			y++;
			j++;
		}

		// On a fini de remplir la ligne

		// On compare

		y = 9;
		j = 0;

		if(strncmp(model, line, strlen(model)) == 0) {
			// La ligne est trouvée		
			return -1;
			break;
		}
	}

	free(line);
	line = NULL;

	return etat;
}

char find_last(char * buffer) {
	int i = 0;

	while (buffer[i+2] != 0) {
		i++;
	}

	return buffer[i];
}

void kill_process (char ** lines, unsigned int size, char pid[100][10]) {
	int i, j, x, y = 0;
	char lastcar;
	
	for (i = 0; i < size; i++) {
		lastcar = find_last(lines[i]);

		if (lastcar != '2') {
			x = 0;
			j = 0;

			while (lines[i][j] != ' ') {
				// Avancement
				j++;
			}

			j++;

			while (lines[i][j] == ' ') {
				// Avancement
				j++;
			}

			while (lines[i][j] != ' ') {
				pid[y][x] = lines[i][j];
				x++;
				j++;
			}

			y++;
		}
	}
}

int camera_enabled (int * exit_script, FILE * LOG) {
	int etat = 0;

	system("gphoto2 --auto-detect > data/tmp/camera.txt");
	system("gphoto2 --debug --debug-logfile=\"data/tmp/claim.txt\" --summary");
	FILE * FIC = fopen("data/tmp/camera.txt", "r");
	FILE * DETECT = fopen("model/detect.txt", "r");
	// Pour la phase de test

	FILE * CLAIM = fopen("model/claim.txt", "r");
	FILE * CLAIM_CONTENT = fopen("data/tmp/claim.txt", "r");

	char ** kill_lines = calloc(100, sizeof(char*));
	char * content = calloc(1000, sizeof(char));
	char model[1000] = "";
	char * claim_content = calloc(1000000, sizeof(char));
	char c;
	int j = 0;
	char * claim = calloc(1000, sizeof(char));
	int claim_status = 0;
	char ** lines = calloc(5000, sizeof(char*));

	for (int i = 0; i < 5000; i++) {
		lines[i] = calloc(1000, sizeof(char));
	}

	for (int i = 0; i < 100; i++) {
		kill_lines[i] = calloc(250, sizeof(char));
	}

	while (!feof(FIC)) {
		c = fgetc(FIC);
		content[j] = c;
		j++;
	}

	j = 0;

	while (!feof(DETECT)) {
		c = fgetc(DETECT);
		model[j] = c;
		j++;
	}

	j = 0;

	while (!feof(CLAIM)) {
		c = fgetc(CLAIM);
		claim[j] = c;
		j++;
	}

	claim[j-1] = 0;

	j = 0;

	while (fgets(lines[j], 1000, CLAIM_CONTENT)) {
		j++;
	}

	claim_status = find(lines, claim, j);

	if (strcmp(content, model) != 0) {
		if (claim_status != -1) {
			etat = 1;
			printf("STATUS OK\n");
			fprintf(LOG, "STATUS OK\n");
		}

		else if (claim_status == -1) {
			// Laisser l'état à 0

			// ECRIRE DANS LE LOG

			// EVENTUELLEMENT ALLUMER UNE LED

			// Lancer le script pour écraser le processus de gphoto2 qui est mal lancé

			j =  0;

			printf("STATUS ERROR\n");
			fprintf(LOG, "STATUS ERROR\nAnalyse des tâches.\n");
			system("ps aux | grep gphoto2 > data/tmp/kill.txt");

			FILE * KILL = fopen("data/tmp/kill.txt", "r");

			while (fgets(kill_lines[j], 249, KILL)) {
				j++;
			}

			char pid[100][10];

			for (int y = 0; y < 100; y++) {
				for (int w = 0; w < 10; w++) {
					pid[y][w] = 0;
				}
			}

			fprintf(LOG, "Destruction des tâches.\n");
			kill_process(kill_lines, j, pid);

			j = 0;

			char * commande = calloc(100, sizeof(char));

			while (pid[j][0] != 0) {
				fprintf(LOG, "Destruction de la tâche au PID %s.\n", pid[j]);
				sprintf(commande, "kill -9 %s", pid[j++]);
				system(commande);
			}

			fclose(KILL);

			free(commande);
			commande = NULL;

			*exit_script = 1;
		}
		
		else {
			// On verra s'il y aura d'autres status à ajouter
		}
	}

	else {
		// Caméra non connectée
		etat = -2;
	}

	for (int i = 0; i < 5000; i++) {
		free(lines[i]);
		lines[i] = NULL;
	}

	for (int i = 0; i < 100; i++) {
		free(kill_lines[i]);
		kill_lines[i] = NULL;
	}

	free(lines);
	lines = NULL;
	free(kill_lines);
	kill_lines = NULL;
	free(content);
	free(claim_content);
	free(claim);
	content = NULL;
	claim_content = NULL;
	claim = NULL;
	fclose(FIC);
	fclose(DETECT);
	fclose(CLAIM);
	fclose(CLAIM_CONTENT);

	// Vider les fichiers temporaires
	system("rm -f data/tmp/claim.txt");
	system("rm -f data/tmp/kill.txt");
	system("rm -f data/tmp/detect.txt");

	return etat;
}

void listen_camera (void) {
	system("cd data/images/tmp;gphoto2 --debug --wait-event-and-download=36000s --debug-logfile=\"../../tmp/capture.txt\";cd ../../..");
}

void send_request (int status) {
    CURL *curl;
    CURLcode res;

    char * request_string = calloc(1000, sizeof(char));
    sprintf(request_string, "status=%d", status);
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/transfert/status");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string);
    
        res = curl_easy_perform(curl); 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    printf ("\n");
}

int main (void) {
	int exit_script = 0;
	int status = 0;
	int disconnect_status = 0;
	int request_sent = 0;

	system("echo \"\" > data/tmp/capture.txt");
	FILE * LOG = fopen("data/log/capture.txt", "a");
	FILE * STATUS = fopen("data/status/camera_connected.txt", "w");

	// J'ai créé deux tâches qu'il faudra gérer et organiser avec FreeRTOS car on est sur un middleware

	// Je règle un délai de 36 000 s au cas où l'autonomie du système est de 10 heures

	// En effet, le délai doit être réglé à la durée d'autonomie du système en secondes

	// J'applique l'ancien DETECTe de fonctionnement des tâches en attendant le développement des middlewares

	while (1) {
		fprintf(LOG, "Obtention du status de la caméra.\n");
		status = camera_enabled(&exit_script, LOG);
		fprintf(STATUS, "%d\n", status);

		if (status == 1) {
			fprintf(LOG, "Caméra connectée.\nLancement du script.\n");
			send_request(1);

			// Activer LED de succès
			// Envoyer un signal à l'application	

			while (1) {
				listen_camera();
				sleep(36000);
			}
		}

		else {
			if (exit_script == 1) {
				fprintf(LOG, "Arrêt du script.\n");
				if (request_sent != 1) {
					send_request(-2);
					request_sent = 1;
				}
			}

			// Activer LED d'erreur
			// Envoyer un signal à l'application
			fprintf(LOG, "En attente de l'activation de la caméra.\n");
			printf ("En attente de l'activation de la caméra.\n");
		}
	}

	fclose(LOG);
	fclose(STATUS);

	return 0;
}
