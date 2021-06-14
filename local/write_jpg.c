#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RATE_INDEX 17451
#define RATE_VALUE 53 // 0

static char image[100] = "/home/hamamji/Bureau/selected/1.JPG";

int min (int * values) {
	int min = values[0];
	for (int i = 1; values[i]; i++) {
		if (min > values[i]) min = values[i];
	}
	return min;
}

int main (int argc, char ** argv) {
	FILE * F;
	FILE * W = fopen("/home/hamamji/Bureau/selected/3.JPG", "w");
	int * indexes = (int*) calloc(10000, sizeof(int));
	int * commonIndexes = (int*) calloc(10000, sizeof(int));
	unsigned char * data = (unsigned char*) calloc(1000000, sizeof(unsigned char));
	int i = 0, x = 0, size;

	F = fopen(image, "r");
	i = 0;

	fseek(F, 0L, SEEK_END);
	size = ftell(F);
	rewind(F);
	while (i < size) {
		data[i++] = fgetc(F);
		printf("%d ", data[i-1]);
	}

	printf("\n");
	int n = i;

	// data[RATE_INDEX] = RATE_VALUE;
	i = 0;

	while (i < n) {
		fputc((char) data[i++], W);
	}

	fclose(W);
	free(indexes);
	indexes = NULL;
	free(data);
	data = NULL;
	free(commonIndexes);
	commonIndexes = NULL;

	return 0;
}
