#include "connexions.h"

void free_usb (Camera * camera, GPContext * context) {
    printf("LIBÉRATION USB ...\n");
    gp_camera_exit(camera, context);
    gp_camera_free(camera);
}
