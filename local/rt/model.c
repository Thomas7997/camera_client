#include "model.h"

// Travailler dessus et vérifier que ca fonctionne
int getModel (char * model, Camera * camera, int * send) {
    int status = 0;
    CameraAbilities abilities;
    char * current_model = (char*) calloc(50, sizeof(char));

    do {
        status = gp_camera_get_abilities (camera, &abilities);

        if (status < 0) return status;

        strcpy(current_model, abilities.model);
        
        if (strcmp(current_model, model) != 0) {
            *send = 1;
        }

        else {
            *send = 0;
        }
    } while (model[0] == 0);

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

    return res;
}

void sendModelHTTP (char * model) {
    // Requête à envoyer au serveur
}