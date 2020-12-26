// Header déstiné au développement des requêtes HTTP et de l'intermédiaire entre scripts C et serveur JS

#ifndef __NET_H__
#define __NET_H__

#include "mods.h"
#include <curl/curl.h>

#define MAX_CAPTURES 10000
#define TAILLE_NOM 100

void send_medias_transfert (char ** files, unsigned int transferts_nb);
void handle_error_net_status(int status);
int send_request (char *name);
// int generateError (int status);

#endif