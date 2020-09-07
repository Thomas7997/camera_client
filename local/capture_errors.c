#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void cut_line (char * cutted, char * line) {
	int x = 0, y = 9;

	while (line[y] != 0) {
		cutted[x] = line[y];
		y++;
		x++;
	}

	printf("CUTTED : %s\nLINE : %s\n", cutted, line);

	return;
}

int check_errors_exec (void) {
	FILE * LOG = fopen("data/tmp/capture.txt", "r");
	FILE * MODEL = fopen("model/shutdown.txt", "r");
	int status = 0;
	int i;
	char * model = calloc(1000, sizeof(char));
	char * line = calloc(1000, sizeof(char));
	char * message = calloc(1000, sizeof(char));

	fgets(model, 44, MODEL);

	while (fgets(line, 999, LOG)) {
		cut_line(message, line);
		if (strncmp(model, message, 44) == 0) {
			status = -1;
			break;
		}
	}

	// Suppression du contenu du fichier (libérer de la mémoire)
	// system("echo \"\" > data/tmp/capture.txt");

	free(line);
	line = NULL;
	free(model);
	model = NULL;
	free(message);
	message = NULL;
	fclose(LOG);
	fclose(MODEL);

	return status;
}

void manage_errors(int * exit_status) {
	int status = check_errors_exec();
	if (status == -1) {
		*exit_status = 1;
	}

	return;
}

int main (void) {
    int exit_script = 0;
    FILE * SCRIPT = fopen("commands/capture.txt", "w");
	FILE * LOG = fopen("data/log/capture_errors.txt", "a");

    while (1) {
        manage_errors(&exit_script);

        if (exit_script == 1) {
            // fprintf(SCRIPT, "0\n");
			fprintf (LOG, "RESTARTING ...\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			system("systemctl --user restart capture");
        }

        else {
            // fprintf(SCRIPT, "1\n");
        }

        // Le script gère les erreures chaque seconde.
	    usleep(500);
    }

    fclose(SCRIPT);
	fclose(LOG);

    return 0;
}