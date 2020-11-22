#include "manage_auto.h"

int main (void) {
    int status = 0, stop = -1;
    FILE * STOP;
    FILE * WIFI;
    FILE * CONNECTED;
    Camera * camera;
    GPContext * context;
    int wifiStatus = 0, successBoot = 0;

    do {
        do {
            gp_camera_new (&camera);
            status = gp_camera_init(camera, context);
            handleError(status);

            if (status < 0) {
                generateError(status);
                // gp_camera_exit(camera, context);
                gp_camera_free(camera);
                successBoot = 0;
                CONNECTED = fopen("data/tmp/camera_connected.txt", "w");
                fprintf(CONNECTED, "0");
                fclose(CONNECTED);
            }

            usleep(5000);
        } while (status != 0);

        if (successBoot == 0) {
            // Indiquer que la connexion est établie correctement
            CONNECTED = fopen("data/tmp/camera_connected.txt", "w");
            fprintf(CONNECTED, "1");
            fclose(CONNECTED);

            WIFI = fopen("data/tmp/wifi_status.txt", "r");
            fscanf(WIFI, "%d", &wifiStatus);

            status = getCameraModel(camera);

            if (status < 0) continue;

            send_status_request(0);
            successBoot = 1;
        }

        char * data;
        CameraEventType e_type;

        status = gp_camera_wait_for_event(camera, 50, &e_type, &data, context);
        handleError(status);

        STOP = fopen("data/stop/transferts.txt", "r");
        fscanf(STOP, "%d", &stop);
    } while (stop == 0);

    fclose(STOP);
    fclose(WIFI);

    return 0;
}