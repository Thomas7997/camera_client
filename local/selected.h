#ifndef SELECTED_H
#define SELECTED_H

#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <cstdlib>

void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
void getPlacements(char * dir, char * file, unsigned int * ratings);

#endif