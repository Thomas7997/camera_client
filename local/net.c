#include "net.h"

int send_status_request (int status) {
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
    return res;
}

int generateError (int status) {
    // Notifier une erreur en fonction de ce qu'il s'agit.

    // J'utilise un système de fichiers et un second script de lecture de fichiers et de lancement de requêtes CURL
    // Écire le status dans un fichier
    FILE * ERR = fopen("data/tmp/errors.txt", "r");
    FILE * NOTIFICATION;
    int previousStatus = 0;

    NOTIFICATION = fopen("data/tmp/errors.txt", "w");
    
    // Envoyer l'ordre de notifier vers un autre script qui détecte le changement de status
    
    fprintf(NOTIFICATION, "%d\n", status);
    fclose(NOTIFICATION);

    printf("GENERATE ERROR %d\n", status);
    fclose(ERR);

    return status;
}

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

void handle_error_net_status(int status) {
    switch (status)
    {
    case 7:
        printf("Problème de connexion.\n");

        // Générer l'alerte sur tout le système de fichiers lié
        
        // Jeu de LEDS disant que la tâche a échoué car il n'y a pas de signal WIFI

        break;
    
    default:
        break;
    }

    printf ("erreur : %d\n", status);
}