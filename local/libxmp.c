#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-file.h>

void handleError(int status) {
    printf ("%s\n", gp_result_as_string(status));
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

int main (int argc, char ** argv) {
    int status;
    GPContext * context;
    Camera * camera;
    char * file = calloc(100, sizeof(char));
    char * dir = calloc(100, sizeof(char));

    do {
        gp_camera_new (&camera);
        status = gp_camera_init(camera, context);
        handleError(status);

        if (status < 0) {
            generateError(status);
            gp_camera_exit(camera, context);
            gp_camera_free(camera);
        }

        usleep(5000);
    } while (status != 0);

    // start
    
    uint64_t size_l = 64000;
      status = gp_camera_file_read(camera,
        dir, // Select one
        file, // Select many
        GP_FILE_TYPE_NORMAL,
        0,
        data,
        &size_l,
        context
    );

    // end

    gp_camera_exit(camera, context);
    gp_camera_free(camera);
    free(file);
    free(dir);

    return 0;
}