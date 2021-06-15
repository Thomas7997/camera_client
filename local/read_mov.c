#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES 512000
#define STR_SIZE (BYTES * 10)

static char images[2][100] = {
	"/home/hamamji/Bureau/selected/1.MOV", // 0
	"/home/hamamji/Bureau/selected/2.MOV"
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
	int * commonIndexes = (int*) calloc(10000, sizeof(int));
	unsigned char * data = (unsigned char*) calloc(STR_SIZE, sizeof(unsigned char));
	unsigned char ** datas = (unsigned char**) calloc(2, sizeof(unsigned char*));
	int i = 0, x = 0, size;
	int * sizes = calloc(2, sizeof(int));

	for (i = 0; i < 2; i++) {
		datas[i] = (char*) calloc(STR_SIZE, sizeof(char));
	}

	for (int img = 0; img < 2; img++) {
		F = fopen(images[img], "r");
		printf("\nVIDEO %d\n\n", img+1);

		i = 0;
		while (i < BYTES) {
			data[i] = fgetc(F);
			datas[img][i] = data[i++];
			printf("%d ", data[i-1]);
		}


		strcpy(data, "");
		fclose(F);
	}

	i = 0;

	for (i = 0; i < BYTES; i++) {
		if (datas[0][i] != datas[1][i]) commonIndexes[x++] = i;
	}

	printf("Common indexes\n");

	for (i = 0; i < x; i++) {
		if (1) printf("%d : %d %d\n", commonIndexes[i], datas[0][commonIndexes[i]], datas[1][commonIndexes[i]]);
	}

	printf("\n");

	for (i = 0; i < 2; i++) {
		free(datas[i]);
		datas[i] = NULL;
	}

	free(sizes);
	sizes = NULL;
	free(data);
	data = NULL;
	free(commonIndexes);
	commonIndexes = NULL;
	free(datas);
	datas = NULL;
	return 0;
}
