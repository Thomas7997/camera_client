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
#include <unistd.h>

void check_error (int status) {
   if (status != 0) printf("%d\n", status);
   else printf ("Réussi\n");
}

int main () {
    GPPort * port;
    GPPortInfo port_info;
    GPPortSettings port_settings;
    int size = 1000;
    char *data = "";
    int status = 0;
    char *path = "";
    char *name = "";
    GPPortInfoList * port_info_list;
    
    port = malloc(sizeof(GPPort));
    port_info = malloc(sizeof(GPPortInfo));
    status = gp_port_info_new(&port_info);
    check_error(status);
    status = gp_port_info_list_new(&port_info_list);
    gp_port_new(&port);
    // status = gp_port_info_set_name(port_info, "serial");
    // check_error(status);
    // status = gp_port_info_set_path(port_info, "/dev/bus/usb/001/017");
    status = gp_port_info_list_load(port_info_list);
    check_error(status);
    gp_port_info_list_get_info(port_info_list,2,&port_info);
    check_error(status);
    status = gp_port_info_get_path(port_info, &path);
    check_error(status);
    printf ("PATH : %s\n", path);
    status = gp_port_info_get_name(port_info, &name);
    check_error(status);
    printf ("NAME : %s\n", name);
    // status = gp_port_info_set_type(port_info, GP_PORT_USB);
    // check_error(status);
    status = gp_port_set_info(port, port_info);
    check_error(status);
    
    // L'union de settings à paramétrer pour les préférences du port USB (comment l'utiliser INT, BULK IN BULK OUT)
    
    status = gp_port_set_settings(port, port_settings);
    check_error(status);

    status = gp_port_open(port);
    check_error(status);

    FILE * FIC = fopen("usbmon.txt", "w");

    while (1) {
        printf ("ANALYSE ...\n");

        status = gp_port_read(port, data, 1000);
        check_error(status);
        fprintf(FIC, "%s\n\n", data);

 	printf ("data : %s\n", data);
        
        usleep(100000);
    }

    gp_port_open(port);
    free(port_info);
    free(port);
    //printf ("%s\n", data);

    gp_port_free(port);
    gp_port_info_list_free(port_info_list);
    gp_port_close(port);
    //free(port_info);
    fclose(FIC);

    return 0;
}
