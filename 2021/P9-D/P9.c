#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME "input.txt"
#define ARRAY_SIZE 100
#define BASIN_COUNT 3

void addBasin(int basins[BASIN_COUNT], int new_basin) {
	for (size_t n = 0; n < BASIN_COUNT; n++) {
		if (new_basin > basins[n]) {
			// move
			for (size_t x = BASIN_COUNT-1; x > n; x--) basins[x] = basins[x-1];
			basins[n] = new_basin;
			return;
		}
	}
}

int getBasinSizeRecursive(char map[ARRAY_SIZE][ARRAY_SIZE], bool visited[ARRAY_SIZE][ARRAY_SIZE], int row, int column) {
	if (visited[row][column] || map[row][column] == 9) return 0;
	
	visited[row][column] = true;
	
	int r = 1;
	if (row > 0) r += getBasinSizeRecursive(map, visited, row-1, column);
	if (row < ARRAY_SIZE-1) r += getBasinSizeRecursive(map, visited, row+1, column);
	if (column > 0) r += getBasinSizeRecursive(map, visited, row, column-1);
	if (column < ARRAY_SIZE-1) r += getBasinSizeRecursive(map, visited, row, column+1);
	return r;
}

int getBasinSize(char map[ARRAY_SIZE][ARRAY_SIZE], int row, int column) {
	bool visited[ARRAY_SIZE][ARRAY_SIZE] = {0};
	return getBasinSizeRecursive(map, visited, row, column);
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char map[ARRAY_SIZE][ARRAY_SIZE];
	for (int row = 0; row < ARRAY_SIZE; row++) {
		for (int column = 0; column < ARRAY_SIZE; column++) {
			fscanf(file, "%c", &map[row][column]);
			map[row][column] -= '0';
		}
		fscanf(file, "%*c"); // '\n'
	}
	
	fclose(file);
	
	int current;
	int basins[BASIN_COUNT] = {0};
	for (int row = 0; row < ARRAY_SIZE; row++) {
		for (int column = 0; column < ARRAY_SIZE; column++) {
			current = map[row][column];
			if (row > 0 && map[row-1][column] <= current) continue;
			if (row < ARRAY_SIZE-1 && map[row+1][column] <= current) continue;
			if (column > 0 && map[row][column-1] <= current) continue;
			if (column < ARRAY_SIZE-1 && map[row][column+1] <= current) continue;
			
			// min point
			addBasin(basins, getBasinSize(map, row, column));
		}
	}
	printf("%ld\n", (unsigned long)basins[0]*basins[1]*basins[2]);
}
