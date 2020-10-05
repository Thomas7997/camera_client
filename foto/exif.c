#include <stdio.h>
#include <stdlib.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
// #include <gphoto2/globals.h>
#include <time.h>
#include <libexif/exif-data.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

#define RATING_TAG 0x4746

/* byte order to use in the EXIF block */
#define FILE_BYTE_ORDER EXIF_BYTE_ORDER_INTEL

/* comment to write into the EXIF block */
#define FILE_COMMENT "libexif demonstration image"

/* special header required for EXIF_TAG_USER_COMMENT */
#define ASCII_COMMENT "ASCII\0\0\0"

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

// static ExifEntry *create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len)
// {
//     void *buf;
//     ExifEntry *entry;

//     /* Create a memory allocator to manage this ExifEntry */
//     ExifMem *mem = exif_mem_new_default();
//     assert(mem != NULL); /* catch an out of memory condition */

//     /* Create a new ExifEntry using our allocator */
//     entry = exif_entry_new_mem (mem);
//     assert(entry != NULL);

//     /* Allocate memory to use for holding the tag data */
//     buf = exif_mem_alloc(mem, len);
//     assert(buf != NULL);

//     /* Fill in the entry */
//     entry->data = (unsigned char*)buf;
//     entry->size = len;
//     entry->tag = tag;
//     entry->components = len;
//     entry->format = EXIF_FORMAT_UNDEFINED;

//     /* Attach the ExifEntry to an IFD */
//     exif_content_add_entry (exif->ifd[ifd], entry);

//     /* The ExifMem and ExifEntry are now owned elsewhere */
//     exif_mem_unref(mem);
//     exif_entry_unref(entry);

//     return entry;
// }

void displayData (FILE * W, unsigned char * data, unsigned int size) {
    for (int i = 0; i < size; i++) {
        fprintf (W, "%d ", data[i]);
    }
}


int main (char argc, char ** argv) {
    CameraFile * file;
    CameraFile * exif;
    unsigned char * data = calloc(1500000, sizeof(unsigned char));
    int size;
    ExifEntry * entry;
    ExifData * ed;
    Camera * camera;
    GPContext *context = sample_create_context();
    unsigned char * buffer = calloc(10000, sizeof(unsigned char));

    int status = 0;

    gp_camera_new (&camera);
    status = gp_camera_init(camera, context);
    handleError(status);

    status = gp_file_new(&file);
    handleError(status);
    status = gp_file_new(&exif);
    handleError(status);
    // status = gp_camera_file_get(camera, argv[1], argv[2], GP_FILE_TYPE_EXIF, exif, context);
    // handleError(status);
    status = gp_camera_file_get(camera, argv[1], argv[2], GP_FILE_TYPE_NORMAL, file, context);
    handleError(status);

    // GP_FILE_TYPE_METADATA
    // GP_FILE_TYPE_NORMAL

    // Récupérer le fichier

    if (!ed) {
        printf ("Erreur, pas de données.\n");
        return 1;
    }

    FILE * DATA = fopen("./img.txt", "w");

    int fd;

    fd = open(argv[2], O_RDWR, 0644);

    status = gp_file_get_data_and_size (file, &data, &size);
    displayData(DATA, data, size+100);
    printf ("%d\n", size);
    handleError(status);
    // ed = exif_data_new_from_data ((unsigned char *)data, size);
    //Set some Exif options
    // exif_data_set_option(ed, EXIF_DATA_OPTION_FOLLOW_SPECIFICATION);
    // exif_data_set_data_type(ed, EXIF_DATA_TYPE_COMPRESSED);
    // exif_data_set_byte_order(ed, FILE_BYTE_ORDER);

    // entry = create_tag(ed, EXIF_IFD_EXIF, RATING_TAG, 
    //         sizeof(ASCII_COMMENT) + sizeof(FILE_COMMENT) - 2);

    // printf ("0\n");
    // entry = exif_entry_new();
    // printf ("0\n");
    // exif_entry_initialize(entry, RATING_TAG);
    // entry->tag = RATING_TAG;
    // exif_content_add_entry (ed->ifd[EXIF_IFD_EXIF], entry);

    // exif_data_dump(ed);

    // if (-1 == lseek(fd, 0, SEEK_SET))
    //     printf ("erreur : lseek\n");
    // if (-1 == write (fd, data, size))
    //     printf("error : write\n");

    // close(fd);

    char * val = calloc(100, sizeof(char));

    // printf("Rating : %s\n", exif_entry_get_value(entry,val,100));

    // exif_entry_unref(entry);

    for (int i = 0; i < EXIF_IFD_COUNT; i++) {
        if (ed->ifd[i]) {
            // show_ifd (ed->ifd[i]);
            // ExifEntry * entry = exif_content_get_entry(content,0x4746);
        }
    }

    gp_file_unref(file);
    gp_file_unref(exif);
    gp_camera_exit(camera, context);
    gp_camera_free(camera);
    free(val);
    free(buffer);
    fclose(DATA);

    return 0;
}
