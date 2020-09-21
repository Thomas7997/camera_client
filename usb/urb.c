#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handleError (int status) {
    const char * error = calloc(100, sizeof(char));
    error = libusb_strerror(status);
    printf ("\nmsg : %s\n", error);
    return;
}

int main (void) {
    int status = 0;
    libusb_device ** list;
    libusb_context * ctx;
    struct libusb_device_descriptor dev_desc;

    libusb_init(&ctx);

    int list_ref = libusb_get_device_list(ctx, &list);
    printf ("%d\n", list_ref);
    status = libusb_get_device_descriptor(list[0], &dev_desc);
    printf ("idProduct %d\nidVendor %d\n\n", dev_desc.idProduct, dev_desc.idVendor);

    libusb_free_device_list(list, list_ref);
    libusb_exit(ctx);
}