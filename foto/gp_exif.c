#include <stdio.h>
#include <stdlib.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
// #include <gphoto2/globals.h>
#include <time.h>
#include <libexif/exif-data.h>
#include "gp-params.h"

int
print_exif (GPParams *p, const char *folder, const char *filename)
{
#ifdef HAVE_LIBEXIF
        CameraFile *file;
        const char *data;
        unsigned long size;
        ExifData *ed;
#ifdef HAVE_LIBEXIF_IFD
	unsigned int i;
#endif

        CR (gp_file_new (&file));
        CRU (gp_camera_file_get (p->camera, folder, filename,
				 GP_FILE_TYPE_EXIF, file, p->context), file);
        CRU (gp_file_get_data_and_size (file, &data, &size), file);
        ed = exif_data_new_from_data ((unsigned char *)data, size);
        gp_file_unref (file);
        if (!ed) {
                gp_context_error (p->context, _("Could not parse EXIF data."));
                return (GP_ERROR);
        }

        printf (_("EXIF tags:"));
        putchar ('\n');
        print_hline ();
        printf ("%-20.20s", _("Tag"));
        printf ("|");
        printf ("%-59.59s", _("Value"));
        putchar ('\n');
        print_hline ();
#ifdef HAVE_LIBEXIF_IFD
	for (i = 0; i < EXIF_IFD_COUNT; i++)
		if (ed->ifd[i]) {
                        show_ifd (ed->ifd[i]);
                        ExifEntry *  	entry = exif_content_get_entry(content,0x4746);
                        printf("Rating : %s\n", exif_entry_get_value(entry,val,100));
                }
#else
        if (ed->ifd0)
                show_ifd (ed->ifd0);
        if (ed->ifd1)
                show_ifd (ed->ifd1);
        if (ed->ifd_exif)
                show_ifd (ed->ifd_exif);
        if (ed->ifd_gps)
                show_ifd (ed->ifd_gps);
        if (ed->ifd_interoperability)
                show_ifd (ed->ifd_interoperability);
#endif
        print_hline ();
        if (ed->size) {
                printf (_("EXIF data contains a thumbnail (%i bytes)."),
                        ed->size);
                putchar ('\n');
        }

        exif_data_unref (ed);

        return (GP_OK);
#else
	gp_context_error (p->context, _("gphoto2 has been compiled without "
		"EXIF support."));
	return (GP_ERROR_NOT_SUPPORTED);
#endif
}

int main (int argc, char **argv, char ** envp) {
    GPParams params;
    CameraList * list;

    gp_list_new(&list);
    gp_list_reset(list);

    gp_params_init(&params, envp);

    if (argc < 3) {
        return 1;
    }

    print_exif(&params, argv[1], argv[2]);
    gp_list_free(list);
    gp_params_exit(&params);

    return 0;
}