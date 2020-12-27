#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <unistd.h>
#include <ctime>
#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <time.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

void handleError(int status);
static void
ctx_error_func (GPContext *context, const char *str, void *data);
static void
ctx_status_func (GPContext *context, const char *str, void *data);
GPContext* sample_create_context();
int send_status_request (int status);

#endif