#include <linux/usb.h>

int main (void) {
    struct usb_device usb_dev;

    char data[1000] = "";
    int * actual_length;


    usb_interrupt_msg(&usb_dev, 1, data, 1000, actual_length, 100);

    return 0;
}