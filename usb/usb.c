#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ltdl.h>
#include <gphoto2/gphoto2-port-result.h>
#include <gphoto2/gphoto2-port-library.h>
#include <gphoto2/gphoto2-port-log.h>
#include "gphoto2-port-info.h"

void handleError(int status) {
    printf ("%s\n", gp_port_result_as_string(status));
}

int main (void) {
    GPPort * port;
    GPPortInfoList * port_infos;
    int status = 0;
    status = gp_port_new(&port);
    handleError(status);
    GPPortInfo port_info;
    status = gp_port_info_new(&port_info);
    handleError(status);

    // status = gp_port_info_set_name(port_info, "Serial");
    // handleError(status);
    // status = gp_port_info_set_type(port_info, GP_PORT_USB);
    // handleError(status);
    // status = gp_port_info_set_path(port_info, "/dev/ttyS0");
    // handleError(status);
    status = gp_port_info_list_new(&port_infos);
    handleError(status);

    status = gp_port_info_list_load(port_infos);
    handleError(status);

    status = gp_port_info_list_get_info(port_infos, 2, &port_info);
    handleError(status);

    char * path = calloc(1000, sizeof(char));

    status = gp_port_info_get_path(port_info, &path);
    printf ("%s\n", path);

    // lib : /usr/lib/arm-linux-gnueabihf/libgphoto2_port/0.12.0/usb1

    status = gp_port_set_info(port, port_info);
    handleError(status);
    status = gp_port_open(port);
    handleError(status);

    char * read = calloc(1000, sizeof(char));

    status = gp_port_set_timeout(port, 5000);
    handleError(status);

    char * c = calloc(2, sizeof(char));

    for (int i = 0; i < 26; i++) {
        status = gp_port_write(port, c, 1);
        handleError(status);
        status = gp_port_read(port, read, 1);
        handleError(status);
        printf ("%s\n", read);
        c[0] += 1;
    }

    status = gp_port_close(port);
    handleError(status);
    status = gp_port_free(port);
    handleError(status);
    free(read);
    free(path);
    free(c);

    return 0;
}