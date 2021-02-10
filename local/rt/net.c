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
        // send_request(files[i]); // Recupérer le status prochainement
        printf("FICHIER %s envoyé.\n", files[i]);
        strcpy(files[i], "");
    }

    free(commande);
}

void send_medias_transfert_online (int online) {
    FILE * NORMAL_FORMAT = fopen("../data/tmp/normal_format.txt", "r");
    FILE * RAW_FORMAT = fopen("../data/tmp/raw_format.txt", "r");
    unsigned int raw_format, normal_format;

    fscanf(RAW_FORMAT, "%u", &raw_format);
    fscanf(NORMAL_FORMAT, "%u", &normal_format);

    int status = 0;
    if (!online) return;
    if (raw_format == 0 && normal_format == 0) return;

    unsigned int i = 0;
    char * commande = (char*) calloc(300, sizeof(char));
    system("ls ../data/images/gets > ../data/images/gets.txt");
    FILE * GETS = fopen("../data/images/gets.txt", "r");

    char ** files = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        files[e] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    while (fgets(files[i], TAILLE_NOM, GETS)) {
        enlever_last_car(files[i++]);
    }

    unsigned int transferts_nb = i;

    for (i = 0; i < transferts_nb; i++) {
        sprintf(commande, "mv ../data/images/gets/%s /home/remote/camera_server/public", files[i]);
        
        system(commande);

        status = send_request(files[i]); // Recupérer le status prochainement
        
        if (status != 0) return;
        
        printf("FICHIER %s envoyé.\n", files[i]);
    }

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        free(files[e]);
    }

    fclose(GETS);
    fclose(NORMAL_FORMAT);
    fclose(RAW_FORMAT);
    free(files);
    free(commande);
}