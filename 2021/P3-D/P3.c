#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BITS 12

/**
 * @param column 	Columna on es troba el bit a extreure (començant per l'esquerra)
 *					/!\ Ha de ser inferior a BITS /!\
 */
#define extractBit(data, column) (( data & (1<<(BITS-column-1)) ) > 0)

typedef enum {
	ONES,
	ZEROS,
	EQUAL
} Predominance;

int btoi(char *txt) {
	int r = 0;
	
	while (*txt) {
		r *= 2;
		r += (*txt == '1');
		txt++;
	}
	
	return r;
}

Predominance getPredominance(int *data, size_t data_size, short column) {
	size_t ones_count = 0;
	for (size_t n = 0; n < data_size; n++) {
		if (extractBit(data[n], column)) ones_count++;
	}
	
	if ((float)ones_count == data_size/2.f) return EQUAL;
	if (ones_count > data_size/2) return ONES;
	return ZEROS;
}

int *getCandidates(int *data, size_t data_size, size_t *candidate_size, short column, bool searching_predominant) {
	// si es busca el predominant, i el predominant son 1 o cap, es busquen els uns
	// si es busca el recessiu, i el predominant son zeros, es busquen els uns
	/* (!searching_predominant && getPredominance(data, data_size, column) == ZEROS) ||
		(searching_predominant && getPredominance(data, data_size, column) != ZEROS) */
	bool find_one = searching_predominant ^ (getPredominance(data, data_size, column) == ZEROS);
	*candidate_size = 0;
	int *r = NULL;
	for (size_t n = 0; n < data_size; n++) {
		// si és 1 i es busca uns, o si és 0 i no es busquen uns
		if (extractBit(data[n], column) == find_one) {
			(*candidate_size)++;
			r = (int*)realloc(r, sizeof(int)*(*candidate_size));
			r[*candidate_size - 1] = data[n];
		}
	}
	return r;
}

int getData(int *data, size_t data_size, bool searching_o2) {
	int *candidates = data, *tmp, r, column = 0;
	size_t candidate_size = data_size;
	while (candidate_size > 1) {
		tmp = getCandidates(candidates, candidate_size, &candidate_size, column, searching_o2);
		if (candidates != data) free(candidates);
		candidates = tmp;
		
		column++;
	}
	
	r = (candidate_size == 1) ? candidates[0] : -1;
	return r;
}

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	size_t data_size = 0;
	int *data = NULL;
	char input[BITS+1];
	fscanf(file, "%s%*c", input);
	while(!feof(file)) {
		data = (int*)realloc(data, sizeof(int)*(++data_size));
		data[data_size-1] = btoi(input);
		
		fscanf(file, "%s%*c", input);
	}
	
	unsigned int o2 = getData(data, data_size, true),
				co2 = getData(data, data_size, false);
	
	printf("%d\n", o2*co2);
	free(file);
	fclose(file);
}
