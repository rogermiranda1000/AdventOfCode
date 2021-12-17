#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define CARBOARD_COLUMNS 5
#define CARBOARD_ROWS 5

#define FILE_NAME "input.txt"

/**
 * @return sempre menor a min_fuel
 */
int getFuel(int *crabs, size_t crab_size, int row, int min_fuel) {
	int fuel = 0, abs;
	for (size_t n = 0; n < crab_size && fuel < min_fuel; n++) {
		abs = crabs[n]-row;
		if (abs < 0) abs *= -1;
		fuel += (abs*(abs+1))/2;
	}
	return fuel;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	int *crabs = NULL;
	size_t crab_size = 0;
	int current;
	int max = 0, min = INT_MAX;
	fscanf(file, "%d%*c", &current);
	while(!feof(file)) {
		crabs = (int*)realloc(crabs, sizeof(int)*(++crab_size));
		crabs[crab_size - 1] = current;
		if (current < min) min = current;
		if (current > max) max = current;
		
		fscanf(file, "%d%*c", &current);
	}
	
	int min_fuel = INT_MAX;
	for (int n = min; n <= max; n++) {
		current = getFuel(crabs, crab_size, n, min_fuel);
		if (current < min_fuel) min_fuel = current;
	}
	printf("%d\n", min_fuel);
	
	fclose(file);
	free(crabs);
}
