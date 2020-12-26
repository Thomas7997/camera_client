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

void send_medias_transfert (char ** files, unsigned int transferts_nb) {
    unsigned int i = 0;
    char * commande = (char*) calloc(300, sizeof(char));

    for (i = 0; i < transferts_nb; i++) {
        sprintf(commande, "mv ../data/images/gets/%s /home/remote/camera_client/public", files[i]);
        send_request(files[i]); // Recupérer le status prochainement
        printf("FICHIER %s envoyé.\n", files[i]);
        strcpy(files[i], "");
    }

    free(commande);
}