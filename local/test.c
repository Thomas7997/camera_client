#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>

#define TMaxL 150000
#define TMax 1000

void handleError(int status) {
    printf ("%d\n", status);
}

static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

GPContext* sample_create_context() {
	GPContext *context;

	/* This is the mandatory part */
	context = gp_context_new();

	/* All the parts below are optional! */
        gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);

	/* also:
	gp_context_set_cancel_func    (p->context, ctx_cancel_func,  p);
        gp_context_set_message_func   (p->context, ctx_message_func, p);
        if (isatty (STDOUT_FILENO))
                gp_context_set_progress_funcs (p->context,
                        ctx_progress_start_func, ctx_progress_update_func,
                        ctx_progress_stop_func, p);
	 */
	return context;
}

int main (void) {
    int status = 0;
    Camera * camera;
    GPContext *context = sample_create_context();
    gp_camera_new (&camera);
    status = gp_camera_init(camera, context);
    handleError(status);
    CameraStorageInformation * storage_info;
    int nrofstorageinformations;

    // TESTS
    // status = gp_filesystem_get_storageinfo (camera->fs, &storage_info, &nrofstorageinformations, context);
    // handleError(status);
    // status = gp_filesystem_make_dir (camera->fs, "/store_00020001", "1", context);
    // handleError(status);
    CameraFile * file;
    gp_file_new(&file);
    status = gp_camera_folder_put_file(camera, "/store_00020001/DCIM/103CANON", "IMG_6185.JPG", GP_FILE_TYPE_NORMAL, file, context);
    handleError(status);
    gp_file_unref(file);
    gp_camera_free(camera);

    return 0;
}