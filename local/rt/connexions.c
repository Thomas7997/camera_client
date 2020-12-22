#include "connexions.h"

void free_usb (Camera * camera, GPContext * context) {
    printf("LIBÉRATION USB ...\n");
    gp_camera_exit(camera, context);
    gp_camera_free(camera);
}

// void camera_usb_connection (Camera * camera, GPContext *context) {
//     int status;
//     while (status != 0) {
//         printf("CONNEXION USB ...\n");
//         gp_camera_new (&camera);
//         status = gp_camera_init(camera, context);
//         // handleError(status);

//         if (status < 0) {
//             // generateError(status);
//             gp_camera_exit(camera, context);
//             gp_camera_free(camera);
//             usleep(100000);
//         }
//     }
// }