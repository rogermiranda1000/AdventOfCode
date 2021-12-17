#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "input.txt"
#define WINDOW 3 // mesures simultaneas en el pitjor dels casos
//#define DEBUG

int sum(int *array, int index) {
	int r = 0;
	for (int x = 0; x < WINDOW; x++) r += array[index + x];
	return r;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_PATH, "r");

	int count = 0;
	size_t size = 0;
	int *data = NULL;
	int tmp;
	fscanf(file, "%d%*c", &tmp);
	while(!feof(file)) {
		data = (int*) realloc(data, sizeof(int)*(++size));
		data[size-1] = tmp;
		
		fscanf(file, "%d%*c", &tmp);
	}
	
	for (int x = 1; x < size - WINDOW + 1; x++) {
		if (sum(data, x) > sum(data, x-1)) count++;
#ifdef DEBUG
		printf("%d (%s)\n", sum(data, x), (sum(data, x) > sum(data, x-1)) ? "increaded" : "decreased");
#endif
	}

	printf("%d\n", count);
	fclose(file);
}
