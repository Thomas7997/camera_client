#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>

void cut_line (char * cutted, char * line) {
	int x = 0, y = 9;

	while (line[y] != 0) {
		cutted[x] = line[y];
		y++;
		x++;
	}
}

int cut_lines (char ** line, unsigned int size, char * model, char * model2) {
	int x, y, i, status = 0;
	char * cutted = calloc(1000, sizeof(char));

	for (i = 0; i < size; i++) {
		x = 0;
		y = 9;
		while (line[i][y] != 0) {
			cutted[x] = line[i][y];
			y++;
			x++;
		}

		if (strncmp(model, cutted, 99) == 0) {
			status = -2;
			printf("%s\n\n%s\n", model2, cutted);
			break;
		}

		else if (strncmp(model2, cutted, 16) == 0) {
			status = -1;
			printf("%s\n\n%s\n", model2, cutted);
			break;
		}
	}

	free(cutted);
	cutted = NULL;

	return status;
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

void clean_lines (void) {
	FILE * FIC = fopen("data/tmp/capture.txt", "w");
	fprintf(FIC, "");
	fclose(FIC);
}

int check_errors_exec (int * iter) {
	FILE * LOG = fopen("data/tmp/capture.txt", "r");
	FILE * MODEL = fopen("model/shutdown.txt", "r");
	FILE * MODEL2 = fopen("model/context.txt", "r");
	int printed = 0;

	int status = 0;
	int i = 0;
	char * model = calloc(1000, sizeof(char));
	char * line = calloc(1000, sizeof(char));
	char * message = calloc(1000, sizeof(char));
	char * model2 = calloc(1000, sizeof(char));
	char ** lines = calloc(100000, sizeof(char*));

	for (int i = 0; i < 100000; i++) {
		lines[i] = calloc(1000, sizeof(char));
	}

	fgets(model, 1000, MODEL);
	fgets(model2, 1000, MODEL2);

	while (fgets(lines[i++], 999, LOG) != NULL) {
		*iter += 1;
	}

	printf ("1\n");
	status = cut_lines(lines, i, model, model2);
	printf ("1\n");

	// while (fgets(line, 999, LOG) != NULL) {
	// 	if (printed == 0) {
	// 		printf ("%s,%d\n", line, printed);
	// 		printed = 1;
	// 	}

	// 	cut_line(message, line);
	// 	// printf("%s\n", message);
	// 	if (strncmp(model, message, 99) == 0) {
	// 		status = -2;
	// 		printf("%s\n\n%s\n", model2, message);
	// 		break;
	// 	}

	// 	else if (strncmp(model2, message, 16) == 0) {
	// 		status = -1;
	// 		printf("%s\n\n%s\n", model2, message);
	// 		break;
	// 	}
	// }

	// Suppression du contenu du fichier (libérer de la mémoire)
	// system("echo \"\" > data/tmp/capture.txt");

	for (int i = 0; i < 100000; i++) {
		free(lines[i]);
		lines[i] = NULL;
	}

	free(line);
	line = NULL;
	free(model);
	model = NULL;
	free(message);
	message = NULL;
	free(model2);
	model2 = NULL;
	fclose(LOG);
	fclose(MODEL);
	fclose(MODEL2);
	free(lines);
	lines = NULL;

	return status;
}

int manage_errors(int * status, int * iter) {
	*status = check_errors_exec(iter);
	printf ("%d\n", *status);
	int exit_status = 0;
	if (*status == -1 || *status == -2) {
		exit_status = 1;
	}

	return exit_status;
}

void reset_log (void) {
	system("echo \"\" > data/tmp/capture.txt");
}

int main (void) {
    int exit_script = 0;
	int status = 0;
	int line_nb = 0;
	FILE * LOG = fopen("data/log/capture_errors.txt", "a");

    while (1) {
        exit_script = manage_errors(&status, &line_nb);
		if (line_nb > 60000) {
			line_nb = 0;
			clean_lines();
		}

        if (exit_script == 1) {
            // fprintf(SCRIPT, "0\n");
			fprintf (LOG, "RESTARTING ...\n\n\n\n\n\n\n\n\n\n\n\n");
			send_request(status);
			// Cette action ne doit pas être répétée trois fois de suite
			system("systemctl --user restart capture");
			line_nb = 0;
			// reset_log();
			usleep(50000);
        }

        else {
            // fprintf(SCRIPT, "1\n");
        }

        // Le script gère les erreures chaque seconde.
	    usleep(500);
    }

	fclose(LOG);

    return 0;
}