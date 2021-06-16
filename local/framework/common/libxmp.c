#include "libxmp.h"

// Functions

int get_jpg_5_stars (const char * data) {
	// Maybe detect a specific val in a large interval

	for (int i = JPG_START_BYTE; i < JPG_BYTE_LENGTH-1; i++) {
		if (data[i] == JPG_5_STARS_VAL) return 1;
	}

	return 0;
}

int get_cr2_5_stars (const char * data) {
	// Maybe detect a specific val in a large interval

	for (int i = JPG_START_BYTE; i < JPG_BYTE_LENGTH-1; i++) {
                if (data[i] == JPG_5_STARS_VAL) return 1;
        }

        return 0;
}

int get_mov_5_stars (const char * data) {
	if (data[0] == MOV_INDEX_0_VAL && data[1] == MOV_INDEX_1_VAL) {
		return 1;
	}

	return 0;
}

int get_mp4_5_stars (const char * data) {
	/* ... */
}
