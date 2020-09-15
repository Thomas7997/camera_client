#include "urb_bf.h"

void display16 (uint8_t * hexa) {
    uint8_t number = 0;

    for (int i = 0; i < 4; i++) {
        printf ("%d ", hexa[i]);
    }

    printf ("\n");
}

void bf (uint8_t * x, uint8_t y) {
    display16(x);
    // Lancer l'URB ici

    if (y == 4) {
        return;
    }

    for (int a = 0; a < 16; a++) {
        x[y] = a;
        bf(x, y+1);
    }
}

void handleError(int status) {
    printf ("%s\n", gp_port_result_as_string(status));
}

int connect_usb(GPPort * port) {
    GPPortInfoList * port_infos;
    int status = 0;
    status = gp_port_new(&port);
    handleError(status);
    GPPortInfo port_info;
    status = gp_port_info_new(&port_info);
    handleError(status);
    status = gp_port_info_list_new(&port_infos);
    handleError(status);

    status = gp_port_info_list_load(port_infos);
    handleError(status);

    status = gp_port_info_list_get_info(port_infos, 2, &port_info);
    handleError(status);

    char * path = calloc(1000, sizeof(char));

    status = gp_port_info_get_path(port_info, &path);
    printf ("%s\n", path);

    status = gp_port_set_info(port, port_info);
    handleError(status);
    status = gp_port_open(port);
    handleError(status);
    return status;
}

int close_port(GPPort * port) {
    status = gp_port_close(port);
    handleError(status);
    status = gp_port_free(port);
    handleError(status);
    return status;
}