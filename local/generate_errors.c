#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send_status_request (int status) {
    CURL *curl;
    CURLcode res;

    char * request_string = (char*) calloc(1000, sizeof(char));
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

int errorsSwitch (int status) {
    if (status == -53 || status == -105 || status == -10 || status == 0) {
        return 1;
    }

    else {
        return 0;
    }
}

int main (void) {
    FILE * ERR;
    FILE * STOP_S;
    FILE * STOP_A;
    int status = 0, previousStatus = 0, stop_s = -1, stop_a = -1;

    ERR = fopen("data/tmp/errors.txt", "r");
    fscanf(ERR, "%d", &status);
    
    if (errorsSwitch(status)) {
        // Notifier
        printf ("Notifier %d\n", status);
        send_status_request(status);
    }

    while (1) {
        // Conditions d'arrêt
        STOP_S = fopen("data/stop/selection.txt", "r");
        STOP_A = fopen("data/stop/transferts.txt", "r");

        fscanf(STOP_S, "%d", &stop_s);
        fscanf(STOP_A, "%d", &stop_a);

        if (stop_a == 0 || stop_s == 0) {
            // Analyse
            ERR = fopen("data/tmp/errors.txt", "r");
            fscanf(ERR, "%d", &status);

            if (previousStatus == 0) {
                previousStatus = status;
            }

            else if (previousStatus != status) {
                if (errorsSwitch(status)) {
                    // Notifier
                    printf ("Notifier %d\n", status);
                    send_status_request(status);
                }

                previousStatus = status;
            }
            fclose(ERR);
        }

        fclose(STOP_A);
        fclose(STOP_S);

        usleep(50000);
    }
    
    return 0;
}