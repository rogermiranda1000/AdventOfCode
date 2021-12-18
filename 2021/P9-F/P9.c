#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME "input.txt"
#define ARRAY_SIZE 100


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
	
	int risk = 0, current;
	for (int row = 0; row < ARRAY_SIZE; row++) {
		for (int column = 0; column < ARRAY_SIZE; column++) {
			current = map[row][column];
			if (row > 0 && map[row-1][column] <= current) continue;
			if (row < ARRAY_SIZE-1 && map[row+1][column] <= current) continue;
			if (column > 0 && map[row][column-1] <= current) continue;
			if (column < ARRAY_SIZE-1 && map[row][column+1] <= current) continue;
			
			// min point
			risk += 1+current;
		}
	}
	printf("%d\n", risk);
}
