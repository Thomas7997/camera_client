#include <stdio.h>
#include <stdlib.h>
#include <gphoto2/gphoto2-camera.h>
#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>
#include <iomanip>

int main (void) {
    int status = 0;
    char data[64000] = "";
    GPContext * context;
    Camera * cam;
    gp_camera_new(&cam);
    do {
        status = gp_camera_init(cam, context);
    } while (status != 0);
    
    uint64_t size_l = 64000;
    status = gp_camera_file_read(cam,
        "/store_00020001/DCIM/103CANON",
        "MVI_6278.MOV",
        GP_FILE_TYPE_NORMAL,
        0,
        data,
        &size_l,
        context
    );

    try {
        unsigned int rating = 0;

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open((const Exiv2::byte*) data, size_l);
    // std::cout << image;
    // assert (image.get() != 0);
    image->readMetadata();
 
        Exiv2::XmpData &xmpData = image->xmpData();
        if (xmpData.empty()) 
        {
            return -1; // Il faudrait trouver un code d'état spécial
            // std::string error(argv[1]);
            // error += ": No XMP properties found in the XMP packet";
            // throw Exiv2::Error(Exiv2::kerErrorMessage, error);
        }
    
        for (Exiv2::XmpData::const_iterator md = xmpData.begin();
            md != xmpData.end(); ++md) {
            std::cout << std::setfill(' ') << std::left
                    << std::setw(44)
                    << md->key() << " "
                    << std::setw(9) << std::setfill(' ') << std::left
                    << md->typeName() << " "
                    << std::dec << std::setw(3)
                    << std::setfill(' ') << std::right
                    << md->count() << "  "
                    << std::dec << md->toString()
                    << std::endl;
        }
            // printf("%d\n", rating);
    }
    
        
    catch (Exiv2::AnyError& e) 
    {
        std::cout << "Caught Exiv2 exception '" << e << "'\n";
        return -1;
    }

    printf("%d\n", status);
    
    return 0;
}