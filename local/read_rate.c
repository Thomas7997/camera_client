#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RATE_INDEX 17451
#define RATE_VALUE 53 // 0

static char image[100] = "/home/hamamji/Bureau/IMG_7367.JPG";

int min (int * values) {
	int min = values[0];
	for (int i = 1; values[i]; i++) {
		if (min > values[i]) min = values[i];
	}
	return min;
}

int main (int argc, char ** argv) {
	FILE * F;
	int * commonIndexes = (int*) calloc(10000, sizeof(int));
	unsigned char * data = (unsigned char*) calloc(5000000, sizeof(unsigned char));
	int i = 0, x = 0, size, index = RATE_INDEX;

	F = fopen(image, "r");
	i = 0;
	fseek(F, 0L, SEEK_END);
        size = ftell(F);
        rewind(F);

	while (i < size) {
		data[i++] = fgetc(F);
		printf("%d ", data[i-1]);
		if (data[i-1] == RATE_VALUE) {
			index = i-1;
			x++;
		}
	}

	i = 0;

	printf("\n\nRate : \n\n");

	printf("%d\nnb : %d", index, x);

	printf("\n");

	fclose(F);
	free(data);
	data = NULL;
	free(commonIndexes);
	commonIndexes = NULL;

	return 0;
}
