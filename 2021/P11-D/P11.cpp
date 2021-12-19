#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>

#define FILE_NAME 	"input.txt"
#define GRID		10

#define THRESHOLD	10

#define ROW_COLUMN_TO_ULL(row,column)	(((unsigned long long)row<<32) | column)
#define ULL_TO_ROW(ull)					((int)(ull>>32))
#define ULL_TO_COLUMN(ull)				((int)ull)

int run(char octopuses[GRID][GRID], bool done[GRID][GRID], int row, int column) {
	if (done[row][column]) return 0; // ja s'ha fet
	
	int flashes = 1;
	done[row][column] = true;
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (y == 0 && x == 0) continue;
			if (row+y < 0 || row+y >= GRID) continue;
			if (column+x < 0 || column+x >= GRID) continue;
			
			octopuses[row+y][column+x]++;
			if (octopuses[row+y][column+x] >= THRESHOLD) flashes += run(octopuses, done, row+y, column+x);
		}
	}
	return flashes;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char octopuses[GRID][GRID];
	
	if (file == NULL) {
		printf("File error\n");
		exit(EXIT_FAILURE);
	}
	
	for (int row = 0; row < GRID; row++) {
		for (int column = 0; column < GRID; column++) {
			fscanf(file, "%c", &octopuses[row][column]);
			octopuses[row][column] -= '0';
		}
		fscanf(file, "%*c"); // '\n'
	}
	
	int step = 0;
	bool all;
	do {
		std::vector<unsigned long long> process;
		bool done[GRID][GRID] = {0};
		
		for (int row = 0; row < GRID; row++) {
			for (int column = 0; column < GRID; column++) {
				octopuses[row][column]++;
				if (octopuses[row][column] >= THRESHOLD) process.push_back(ROW_COLUMN_TO_ULL(row, column));
			}
		}
		
		for (auto &e : process) {
			run(octopuses, done, ULL_TO_ROW(e), ULL_TO_COLUMN(e));
		}
		
		// reiniciar els flashed
		all = true;
		step++;
		for (int row = 0; row < GRID; row++) {
			for (int column = 0; column < GRID; column++) {
				if (octopuses[row][column] >= THRESHOLD) octopuses[row][column] = 0;
				else all = false;
			}
		}
	} while (!all);

	printf("%d\n", step);
	fclose(file);
}
