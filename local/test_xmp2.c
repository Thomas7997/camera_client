#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void) {    
    for (int i = 0; i < 200; i++) {
        system("cd ../foto;./exiv_xmp /store_00020001/DCIM/101CANON IMG_6188.JPG");
    }

    return 0;
}