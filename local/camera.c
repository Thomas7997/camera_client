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
#include <gphoto2/gphoto2-port-library.h>

#ifndef GPHOTO_PORT_INFO_H
#define GPHOTO_PORT_INFO_H

struct _GPPortInfo {
 	GPPortType type;        
        char *name;             
        char *path;             
        /* Private */
        char *library_filename; 
};

#endif

int main () {
	// CAPTURES_TYPES : GP_CAPTURE_IMAGE || GP_CAPTURE_MOVIE || GP_CAPTURE_SOUND

	Camera *camera;

	struct _GPPortInfo port_info;
	
	printf ("1");

	port_info.type = GP_PORT_USB ;

	strcpy(port_info.name,"usb:");

	strcpy(port_info.path,"usb:001,004");

	GPPort* port;

	char data[100000] = "";

	gp_port_new(&port);

	printf ("2");

	gp_port_set_info(port, port_info);

	printf ("1");

	gp_port_read(port, data, 100000);	

	printf ("%s\n", data);

	return 0;
}
