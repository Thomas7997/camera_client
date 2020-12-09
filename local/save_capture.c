#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <gphoto2/gphoto2.h>

#include "samples.h"

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

static void
errordumper(GPLogLevel level, const char *domain, const char *str, void *data) {
	fprintf(stderr, "%s\n", str);
}

static struct queue_entry {
	CameraFilePath	path;
	int offset;
} *queue = NULL;
static int nrofqueue=0;
static int nrdownloads=0;

static const char *buffer;

static int
wait_event_and_download (Camera *camera, int waittime, GPContext *context) {
	CameraEventType	evtype;
	CameraFilePath	*path;
	void		*data;
	int		retval;
        struct timeval	start, curtime;

        gettimeofday (&start, NULL);
	data = NULL;

	while (1) {
		retval = gp_camera_wait_for_event(camera, 100, &evtype, &data, context);
		if (retval != GP_OK) {
			fprintf (stderr, "return from waitevent in trigger sample with %d\n", retval);
			return retval;
		}
		
		if (evtype == GP_EVENT_FILE_CHANGED) {
            path = data;
            printf ("%s\n", changed);
        }

        else if (evtype == GP_EVENT_CAPTURE_COMPLETE) {

        }
	}
	if (nrofqueue) {
		unsigned long	size;
		int		fd;
		struct stat	stbuf;
		CameraFile	*file;

		retval = gp_file_new(&file);

		fprintf(stderr,"camera getfile of %s / %s\n",
			queue[0].path.folder,
			queue[0].path.name
		);
		retval = gp_camera_file_get(camera, queue[0].path.folder, queue[0].path.name,
			GP_FILE_TYPE_NORMAL, file, context);
		if (retval != GP_OK) {
			fprintf (stderr,"gp_camera_file_get failed: %d\n", retval);
			gp_file_free (file);
			return retval;
		}

		/* buffer is returned as pointer, not as a copy */
		retval = gp_file_get_data_and_size (file, &buffer, &size);

		if (retval != GP_OK) {
			fprintf (stderr,"gp_file_get_data_and_size failed: %d\n", retval);
			gp_file_free (file);
			return retval;
		}
		if (-1 == stat(queue[0].path.name, &stbuf))
			fd = creat(queue[0].path.name, 0644);
		else
			fd = open(queue[0].path.name, O_RDWR | O_BINARY, 0644);
		if (fd == -1) {
			perror(queue[0].path.name);
			return GP_ERROR;
		}
		if (-1 == lseek(fd, 0, SEEK_SET))
			perror("lseek");
		if (-1 == write (fd, buffer, size))
			perror("write");
		close (fd);

		gp_file_free (file); /* Note: this invalidates buffer. */

		fprintf(stderr,"ending download %d, deleting file.\n", nrdownloads);
		retval = gp_camera_file_delete(camera, queue[0].path.folder, queue[0].path.name, context);
		memmove(&queue[0],&queue[1],sizeof(queue[0])*(nrofqueue-1));
		nrofqueue--;
	}
	return GP_OK;
}

int
main(int argc, char **argv) {
	Camera		*camera;
	int		retval, nrcapture = 0;
	struct timeval	tval;
	GPContext 	*context = sample_create_context();

	gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
	/*gp_log_add_func(GP_LOG_DATA, errordumper, NULL); */
	gp_camera_new(&camera);

	retval = gp_camera_init(camera, context);
	if (retval != GP_OK) {
		printf("gp_camera_init: %d\n", retval);
		exit (1);
	}
	while (1) {
		retval = wait_event_and_download(camera, 100, context);
		if (retval != GP_OK)
			break;
	}
	gp_camera_exit(camera, context);
	return 0;
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
