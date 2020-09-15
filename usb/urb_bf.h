#ifndef URB_BF_H
#define URB_BF_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ltdl.h>
#include <gphoto2/gphoto2-port-result.h>
// #include <gphoto2/gphoto2-port-library.h>
#include <gphoto2/gphoto2-port-log.h>
#include "gphoto2-port-info.h"

int close_port(GPPort * port);
int connect_usb(GPPort * port);
void handleError(int status);
void bf (uint8_t * x, uint8_t y);
void display16 (uint8_t * hexa);

#endif