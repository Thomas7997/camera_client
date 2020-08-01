#include <stdio.h>
#include "Class/Host/CDC.h"

int main () {
    USB_ClassInfo_CDC_Host_t usb_device;

    // Paramétrer usb_device ...
    usb_device.Config.DataINPipeNumber = 0;
    usb_device.Config.DataINPipeDoubleBank = 1;
    usb_device.Config.DataOUTPipeNumber = 0;
    usb_device.Config.DataOUTPipeDoubleBank = 1;
    usb_device.Config.NotificationPipeNumber = 0;
    usb_device.Config.NotificationPipeDoubleBank = 1;

    int result = 0;
    FILE* LOG = fopen("./usb_cam_debug.txt", "w");

    CDC_Host_ConfigurePipes(&usb_device, 1000, "Réceptions");

    while (1) {
        result = CDC_Host_USBTask(&usb_dev);

        // Possibles erreures
        fprintf(LOG, "IN ORDER ->> %s\nResult ->> %d\n", &usb_device->State.ControlLineStates->DeviceToHost, result);
        printf(LOG, "IN ORDER ->> %s\nResult ->> %d\n", &usb_device->State.ControlLineStates->DeviceToHost, result);

        if (result < 0) {
            break;
            return 1;
        }

        sleep(1);
    }

    fclose(LOG);

    return 0;
}