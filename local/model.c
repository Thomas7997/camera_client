#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

int main (void) {
    char * model = calloc(50, sizeof(char));
    int stop = -1;

    do {
        FILE * STOP = fopen("data/stop/transferts.txt", "r");
        FILE * MODEL = fopen("data/tmp/model.txt", "r");
        fscanf(STOP, "%d", &stop);
        strcpy(model, "");
        fgets(model, 50, MODEL);

        if (model[0] != 0) {
            CURL *curl;
            CURLcode res;

            char * request_string = (char*) calloc(1000, sizeof(char));
            sprintf(request_string, "model=%s", model);
            
            curl_global_init(CURL_GLOBAL_ALL);
            
            curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/transfert/camera");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string);
            
                res = curl_easy_perform(curl);
                if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            
                curl_easy_cleanup(curl);
            }
            curl_global_cleanup();
            printf ("\n");
            fclose(MODEL);
            fclose(STOP);
            usleep(500);
        }
    } while (stop == 0);

    free(model);

    return 0;
}