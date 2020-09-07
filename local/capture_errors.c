#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int check_errors_exec (void) {
	FILE * LOG = fopen("data/tmp/capture.txt", "r");
	FILE * MODEL = fopen("model/shutdown.txt", "r");
	int status = 0;
	int i;
	char * model = calloc(1000, sizeof(char));
	char * line = calloc(1000, sizeof(char));

	while (fgets(line, 999, LOG)) {
		if (strncmp(model, line, 44) == 0) {
			status = -1;
			break;
		}
	}

	// Suppression du contenu du fichier (libérer de la mémoire)
	system("echo \"\" > data/tmp/capture.txt");

	free(line);
	line = NULL;
	free(model);
	model = NULL;
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

    while (1) {
        manage_errors(&exit_script);

        if (exit_script == 1) {
            fprintf(SCRIPT, "0\n");
        }

        else {
            fprintf(SCRIPT, "1\n");
        }

        // Le script gère les erreures chaque seconde.
	    sleep(1);
    }

    fclose(SCRIPT);

    return 0;
}