// Header déstiné au développement des requêtes HTTP et de l'intermédiaire entre scripts C et serveur JS

#ifndef __NET_H__
#define __NET_H__

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPTURES 10000
#define TAILLE_NOM 100

int send_request (char *name);

#endif