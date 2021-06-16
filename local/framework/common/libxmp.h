#ifndef __LIB_XMP_H__
#define __LIB_XMP_H__

// Images parsing
// JPG / JPEG
#define JPG_START_BYTE 0
#define JPG_BYTE_LENGTH 64000
#define JPG_5_STARS_VAL 53

// CR2

// NEF

// Videos parsing
// MOV
#define MOV_START_BYTE 65902 // RATES indexes ...
#define MOV_BYTE_LENGTH 2
#define MOV_INDEX_0_VAL	64
#define MOV_INDEX_1_VAL 160

// MP4

// Functions
int get_jpg_5_stars (const char * data);
int get_cr2_5_stars (const char * data);
int get_mov_5_stars (const char * data);
int get_mp4_5_stars (const char * data);

#endif
