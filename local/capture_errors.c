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

	return;
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

int check_errors_exec (void) {
	FILE * LOG = fopen("data/tmp/capture.txt", "r");
	FILE * MODEL = fopen("model/shutdown.txt", "r");
	FILE * MODEL2 = fopen("model/claim.txt", "r");
	FILE * MODEL3 = fopen("model/change.txt", "r");
	int status = 0;
	int i;
	char * model = calloc(1000, sizeof(char));
	char * line = calloc(1000, sizeof(char));
	char * message = calloc(1000, sizeof(char));
	char * model2 = calloc(1000, sizeof(char));
	char * model3 = calloc(1000, sizeof(char));

	fgets(model, 1000, MODEL);
	fgets(model2, 1000, MODEL2);
	fgets(model3, 1000, MODEL3);

	while (fgets(line, 999, LOG)) {
		cut_line(message, line);
		// printf("%s\n", message);
		if (strncmp(model, message, 99) == 0) {
			status = -2;
			printf("%s\n\n%s\n", model2, message);
			break;
		}

		else if (strncmp(model2, message, 333) == 0 || strncmp(model3, message, 89) == 0) {
			status = -1;
			printf("%s\n\n%s\n", model2, message);
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
	free(model2);
	model2 = NULL;
	free(model3);
	model3 = NULL;
	fclose(LOG);
	fclose(MODEL);
	fclose(MODEL2);
	fclose(MODEL3);

	return status;
}

int manage_errors(int * status) {
	*status = check_errors_exec();
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
	FILE * LOG = fopen("data/log/capture_errors.txt", "a");

    while (1) {
        exit_script = manage_errors(&status);

        if (exit_script == 1) {
            // fprintf(SCRIPT, "0\n");
			fprintf (LOG, "RESTARTING ...\n\n\n\n\n\n\n\n\n\n\n\n");
			send_request(status);
			// Cette action ne doit pas être répétée trois fois de suite
			system("systemctl --user restart capture");
			reset_log();
			sleep(3);
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