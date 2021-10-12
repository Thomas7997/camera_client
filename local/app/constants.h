#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// CONSTANTES
#define MAX 80
#define MAX_CAPTURES 10000
#define MAX_DIR_CAPTURES 100
#define TAILLE_NOM 100
#define MIN_DIRS 10
#define NB_PARTS 2
#define PART_NB 49
#define MAX_DOWNLOADS 100

// OP status
#define OP_OFFLINE_SUCCESS 2 // LED
#define OP_NO_STATUS 1
#define OP_OK 0
#define OP_WIFI_ERROR -1 // LED
#define OP_USB_ERROR -2 // LED or ALERT

// OP mode
#define OP_NONE 0
#define OP_DOWNLOAD 1
#define OP_DELETE 2

#endif