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

int main () {
	CameraFilePath path = { "TEST.jpg", "/" };
	CameraAbilities abilities = {
		"Canon EOS 1300D",
		GP_DRIVER_STATUS_PRODUCTION,
		GP_PORT_USB,
		38400
	};
	printf("1\n");
	CameraList *list[5];

	// CAPTURES_TYPES : GP_CAPTURE_IMAGE || GP_CAPTURE_MOVIE || GP_CAPTURE_SOUND

	Camera *camera;
	GPContext* context = gp_context_new(); 	

	gp_camera_new (&camera);

	gp_camera_set_abilities(camera, abilities); 	

	camera = list[0];

	printf("1\n");

	gp_camera_init(camera, context);

	printf("1\n");

	gp_camera_capture(camera, GP_CAPTURE_IMAGE, &path, context);

	return 0;
}