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
 
int main(int argc, char** argv)
{
    int status = 0;
    CameraFile * file;
    const char * data = (const char*) malloc(1500000 * sizeof(char));
    unsigned long size;
    Camera * camera;
    GPContext *context = sample_create_context();

    gp_camera_new (&camera);
    status = gp_camera_init(camera, context);
    status = gp_file_new(&file);
    status = gp_camera_file_get(camera, argv[1], argv[2], GP_FILE_TYPE_NORMAL, file, context);
    status = gp_file_get_data_and_size (file, (const char**) &data, &size);

    // const std::vector<uint8_t> bytes = std::vector<uint8_t> vec(str.begin(), str.end());

    // std::vector<uint8_t> bytes = static_assert(std::is_same<uint8_t, char>::value, "uint8_t is not unsigned char");
    

  Exiv2::XmpParser::initialize();
  ::atexit(Exiv2::XmpParser::terminate);
 
  try
  {
    if (argc != 3) 
      {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
      }

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open((const Exiv2::byte*) data, size);
    // std::cout << image;
    // assert (image.get() != 0);
    image->readMetadata();
 
    Exiv2::XmpData &xmpData = image->xmpData();
    if (xmpData.empty()) {
        std::string error(argv[1]);
        error += ": No XMP data found in the file";
        throw Exiv2::Error(Exiv2::kerErrorMessage, error);
    }
    if (xmpData.empty()) 
      {
        std::string error(argv[1]);
        error += ": No XMP properties found in the XMP packet";
        throw Exiv2::Error(Exiv2::kerErrorMessage, error);
      }
 
    for (Exiv2::XmpData::const_iterator md = xmpData.begin();
         md != xmpData.end(); ++md) 
      {
        std::cout //<< std::setfill(' ') << std::left
        //           << std::setw(44)
        //           << md->key() << " "
        //           << std::setw(9) << std::setfill(' ') << std::left
        //           << md->typeName() << " "
        //           << std::dec << std::setw(3)
        //           << std::setfill(' ') << std::right
        //           << md->count() << "\n"
                  << std::dec << md->toString()
                  << std::endl;
      }
 
    Exiv2::XmpParser::terminate();
 
    return 0;
  }
catch (Exiv2::AnyError& e) 
  {
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return -1;
  }
}

// Compilation : g++ -o exiv_xmp exiv_xmp.cpp -lexiv2 -lgphoto2 -lgphoto2_port