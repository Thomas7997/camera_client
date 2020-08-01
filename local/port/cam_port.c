#include <gphoto2/gphoto2-camera.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <gphoto2/gphoto2-result.h>
#include <gphoto2/gphoto2-library.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-context.h>
#include <gphoto2/gphoto2-abilities-list.h>
#include <gphoto2/gphoto2-port.h>
#include <gphoto2/gphoto2-widget.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-result.h>
#include <gphoto2/gphoto2-port-result.h>
#include <gphoto2/gphoto2-port.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-port-result.h>
#include <gphoto2/gphoto2-port-info-list.h>
#include <gphoto2/gphoto2-port-portability.h>

int main () {
    int idvendor = 0x04a9;
    int idproduct = 0x32b4;

    GPPort * port;
    int size = 1000;
    char data[1000] = "";
    int status = 0;

    if (gp_port_new(&port) == GP_OK) printf ("Réussi\n");

    status = gp_port_usb_find_device(port,idvendor,idproduct);

    if (status != 0) {
        printf ("%d\n", status);
    }
    
    status = gp_port_read(port, data, size);

    if (status != 0) {
        printf ("%d\n", status);
    }

    printf ("%s\n", data);

    gp_port_free(port);

    return 0;
}