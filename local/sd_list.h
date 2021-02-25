#ifndef __SD_LIST_H__
#define __SD_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>

int for_each_folder (GPParams *, FolderAction action);
int for_each_file (GPParams *, FileAction action);
int for_each_file_in_range  (GPParams *, FileAction action, const char *range);

#endif