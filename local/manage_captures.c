#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#define MAX 80 

#define MAX_CAPTURES 10000
#define TAILLE_NOM 30

void mirroir (char *chaine);

void send_base64_request (char *base64) {
    CURL *curl;
    CURLcode res;

    char * request_string = calloc(10000000, sizeof(char));
    sprintf(request_string, "data=%s", base64);
    
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
}

void vider_buffer (char *buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = 0;
    }
}

void afficher_tab2 (char tab[MAX_CAPTURES][TAILLE_NOM]) {
    for (int i = 0; i < MAX_CAPTURES; i++) {
        for (int j = 0; j < TAILLE_NOM; j++) {
            printf ("%c", tab[i][j]);
        }
        printf ("\n");
    }
}

// Il faut enlever l'extension
void transform_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char nouvelleListe[MAX_CAPTURES][TAILLE_NOM], int size) {
    int i = 0, j;

    for (i = 0; i < size; i++) {
        j = 0;
        while (liste[i][j] != '.') {
            nouvelleListe[i][j] = liste[i][j];
            printf ("%c", nouvelleListe[i][j]);
            j++;
        }
        printf ("\n");
    }
}

void linearize (char *base, char **lines) {
    int i = 0, j = 0, x = 0;

    while (lines[i][0] != 0) {
        j = 0;
        while (lines[i][j] != 0 && lines[i][j+1] != '\n') {
            base[x++] = lines[i][j++];
        }
        i++;
    }
}

void transferer_noms (char liste[MAX_CAPTURES][TAILLE_NOM], char old[MAX_CAPTURES][TAILLE_NOM]) {
    int i = 0, j;

    char commande[250] = "";
    char path[50] = "";
    printf ("1\n");

    while (liste[i][0] != 0) {
        sprintf(commande, "base64 ./data/images/tmp/%s > ./data/datas/tmp/%s.txt", old[i], liste[i]);
        // sprintf(commande, "mv ./data/capture/%s ./data/capture/cloud/%s;mv ./data/capture/cloud/%s ./data/images/cloud", old[i], liste[i], liste[i]);
    	// system(commande);
        // Envoyer le nom du nouveau du nouveau fichier transféré au socket
        system(commande);
        
        printf ("2\n");

        sprintf(path, "./data/datas/tmp/%s.txt", liste[i]);

        FILE * FIC = fopen(path, "r");
        char ** lines = calloc(1000000, sizeof(char*));

        for (int x = 0; x < 1000000; x++) {
            lines[x] = calloc(100, sizeof(char));
        }

        printf ("3\n");

        j = 0;

        if (FIC == NULL) {
            printf ("FICHIER INCORRECTE\n%s\n", path);
            exit(1);
        }

        char c;

        int z = 0;

        while (fgets(lines[j], 100, FIC)) {
            // Remplissage
            printf("%s\n", lines[j]);
            j++;
        }

        printf ("1\n");

        char * base64 = calloc(10000000, sizeof(char));

        FILE * LOG = fopen("./log.txt", "w");

        printf ("11\n");

        linearize(base64, lines);

        base64[j-2] = 0;

        fclose(FIC);

        printf ("4\n");

        // sprintf(envoi, "curl -d '{\"data\":\"data:image/jpeg;base64,%s\"}' -H \"Content-Type: application/json\" -X POST http://localhost:8000/transfert/photo\nrm -f ./data/datas/tmp/%s.txt ./data/images/tmp/%s", base64, liste[i], old[i]);

        // sprintf(envoi, "rm -f ./data/datas/tmp/%s.txt ./data/images/tmp/%s", liste[i], old[i]);

        printf ("%s\n", base64);

        fprintf(LOG, "%s\n", base64);
        fclose(LOG);

        send_base64_request(base64);

        // Peut etre vider le buffer avant avec une fonction créée.

        for (int x = 0; x < 1000000; x++) {
            free(lines[x]);
            lines[x] = NULL;
        }

        fclose(FIC);
        free(lines);
        free(base64);
        lines = NULL;
        base64 = NULL;
        
        i++;
    }
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

int main (void) {
    system("cd data/images/tmp;ls capt* > ../liste.txt;cd ../../..");

    FILE * CAPTURES = fopen("./data/images/liste.txt", "r");

    printf ("1\n");

    char liste_captures[MAX_CAPTURES][TAILLE_NOM];
    char nouvelles_captures[MAX_CAPTURES][TAILLE_NOM];

    int i, j, number = 0;

    for (i = 0; i < MAX_CAPTURES; i++) {
        for (j = 0; j < TAILLE_NOM; j++) {
            liste_captures[i][j] = 0;
            nouvelles_captures[i][j] = 0;
	    }
    }

    i = 0;

    while (fgets(liste_captures[i], TAILLE_NOM, CAPTURES) != NULL) {
        // Remplissage
        enlever_last_car(liste_captures[i++]);
        number++;
    }

    if (strcmp(liste_captures[i], "ls: cannot access 'cloud/capt*': No such file or directory") == 0) {
	    return 1;
    }

    i = 0;

    printf ("1\n");

    transform_noms(liste_captures, nouvelles_captures, number);

    printf("%d\n", number);

    transferer_noms(nouvelles_captures, liste_captures);

    fclose(CAPTURES);

    system("ls ./data/images/tmp > ./data/images/liste.txt");

    return 0;
}
