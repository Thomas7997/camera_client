#include "net.h"

// Envoi de la photo
int send_request (char *name) {
    printf("Requêtes\n");
    
    CURL *curl;
    CURLcode res;

    char * request_string = (char*) calloc(1000, sizeof(char));
    sprintf(request_string, "name=%s", name);

    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/transfert/photo");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string);
    
        res = curl_easy_perform(curl); 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    free(request_string);
    printf ("\n");
    return res;
}