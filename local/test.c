#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libudev.h>
#include <string.h>

/*

    RESULT OF LISTING without external device (RaspberryPi3 B+) :

    /sys/devices/platform/soc/3f980000.usb/usb1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-0:1.0
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.1/1-1.1:1.0

    RESULT OF LISTING without external device (RaspberryPi3 B+) :

    /sys/devices/platform/soc/3f980000.usb/usb1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-0:1.0
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.1
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.1/1-1.1:1.0
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0
    /sys/devices/platform/soc/3f980000.usb/usb1/1-1/1-1:1.0

*/



int lsusb_find_camera (char * path) {
    system("lsusb > ./data/tmp/lsusb.txt");
    FILE * LSUSB = fopen("./data/tmp/lsusb.txt", "r");
    char ** lines = calloc(30, sizeof(char*));
    int x = 0;

    for (int i = 0; i < 30; i++) {
        lines[i] = calloc(100, sizeof(char));
    }

    while (fgets(lines[x++], 99, LSUSB));

    int size = x;
    char * nmbrStr = calloc(5, sizeof(char));

    for (x = 0; x < size-1; x++) {
        // Ports 1, 2, 3 or reserved
        int y = 0;
        strcpy(nmbrStr, "");
        while (y<15) y++;
        // while (lines[x][y++] != ' ');
        for (int c = 0; c < 3; c++) nmbrStr[c] = lines[x][y+c];

        if (strcmp(nmbrStr, "001") && strcmp(nmbrStr, "002") && strcmp(nmbrStr, "003")) {
            sprintf(path, "/dev/bus/usb/001/%s", nmbrStr);
            return 0;
        }
    }

    free(nmbrStr);

    for (int i = 0; i < 30; i++) {
        free(lines[i]);
    }

    fclose (LSUSB);
    free(lines);

    return -1;
}

int main (void) {
    char * path = calloc(100, sizeof(char));
    int ret = lsusb_find_camera (path);

    printf("%s\n", path);

    free(path);

    return 0;
}