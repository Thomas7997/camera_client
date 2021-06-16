#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char images[2][100] = {
	"/home/hamamji/Bureau/selected/1.JPG", // 0
	"/home/hamamji/Bureau/selected/2.JPG"
};

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
	unsigned char * data = (unsigned char*) calloc(5000000, sizeof(unsigned char));
	unsigned char ** datas = (unsigned char**) calloc(2, sizeof(unsigned char*));
	int i = 0, x = 0, size;
	int * sizes = calloc(2, sizeof(int));

	for (i = 0; i < 2; i++) {
		datas[i] = (char*) calloc(5000000, sizeof(char));
	}

	for (int img = 0; img < 2; img++) {
		F = fopen(images[img], "r");
		printf("\nIMAGE %d\n\n", img+1);

		i = 0;
		while (i < 512000) {
			data[i] = fgetc(F);
			datas[img][i] = data[i++];
			printf("%d ", data[i-1]);
		}


		strcpy(data, "");
		fclose(F);
	}

	i = 0;

	for (i = 0; i < 512000; i++) {
		if (datas[0][i] != datas[1][i]) commonIndexes[x++] = i;
	}

	printf("Common indexes\n");

	for (i = 0; i < x; i++) {
		if (1) printf("%d : %d %d\n", commonIndexes[i], datas[0][commonIndexes[i]], datas[1][commonIndexes[i]]); // Also check for the previous and next indexes
	}

	printf("\n");

	for (i = 0; i < 2; i++) {
		free(datas[i]);
		datas[i] = NULL;
	}

	free(sizes);
	sizes = NULL;
	fclose(W);
	free(indexes);
	indexes = NULL;
	free(data);
	data = NULL;
	free(commonIndexes);
	commonIndexes = NULL;
	free(datas);
	datas = NULL;
	return 0;
}
