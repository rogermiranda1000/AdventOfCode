#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CARBOARD_COLUMNS 5
#define CARBOARD_ROWS 5

#define FILE_NAME "input.txt"

typedef enum {
	EMPTY = '.',
	RIGHT = '>',
	DOWN = 'v'
} Cucumbers;

typedef struct {
	Cucumbers **land;
	size_t rows, columns;
} Land;

void freeLand(Land *land) {
	for (size_t n = 0; n < land->rows; n++) free(land->land[n]);
	free(land->land);
}

Land getLand(char *file_name) {
	FILE *file = NULL;
	file = fopen(file_name, "r");
	Land r = (Land){NULL,0,0};
	
	char tmp;
	fscanf(file, "%c", &tmp);
	if (feof(file) || tmp == '\n') {
		fclose(file);
		return r;
	}
	
	r.land = (Cucumbers**)malloc(sizeof(Cucumbers*)*(++r.rows));
	r.land[r.rows - 1] = NULL;
	while (!feof(file) && tmp != '\n') {
		r.land[r.rows - 1] = (Cucumbers*)realloc(r.land[r.rows - 1], sizeof(Cucumbers)*(++r.columns));
		r.land[r.rows - 1][r.columns - 1] = (Cucumbers)tmp;
		
		fscanf(file, "%c", &tmp);
	}
	
	while (!feof(file)) {
		r.land = (Cucumbers**)realloc(r.land, sizeof(Cucumbers*)*(++r.rows));
		r.land[r.rows - 1] = (Cucumbers*)malloc(sizeof(Cucumbers)*r.columns);
		for (size_t n = 0; n < r.columns; n++) {
			fscanf(file, "%c", &tmp);
			r.land[r.rows - 1][n] = (Cucumbers)tmp;
		}
		fscanf(file, "%*[\n]"); // '\n'
	}
	
	fclose(file);
	return r;
}

void allocateLand(Land *land) {
	land->land = (Cucumbers**)malloc(sizeof(Cucumbers*)*land->rows);
	for (size_t row = 0; row < land->rows; row++) land->land[row] = (Cucumbers*)malloc(sizeof(Cucumbers)*land->columns);
}

bool leftTurn(Land l, Land *result) {
	bool change = false;
	*result = (Land){NULL, l.rows, l.columns};
	allocateLand(result);
	
	for (size_t y = 0; y < l.rows; y++) {
		for (size_t x = 0; x < l.columns; x++) {
			if (l.land[y][x] == RIGHT && l.land[y][(x+1)%l.columns] == EMPTY) {
				result->land[y][(x+1)%l.columns] = RIGHT;
				result->land[y][x] = EMPTY;
				x++; // don't change it on the next iteration
				change = true;
			}
			else result->land[y][x] = l.land[y][x]; // it should skip, but we need to copy
		}
	}
	
	return change;
}

bool downTurn(Land l, Land *result) {
	bool change = false;
	*result = (Land){NULL, l.rows, l.columns};
	allocateLand(result);
	
	bool setted[l.columns];
	for (size_t n = 0; n < l.columns; n++) setted[n] = false;
	for (size_t y = 0; y < l.rows; y++) {
		for (size_t x = 0; x < l.columns; x++) {
			if (l.land[y][x] == DOWN && l.land[(y+1)%l.rows][x] == EMPTY) {
				result->land[(y+1)%l.rows][x] = DOWN;
				result->land[y][x] = EMPTY;
				setted[x] = true; // don't change it on the next iteration
				change = true;
			}
			else {
				if (setted[x]) setted[x] = false;
				else result->land[y][x] = l.land[y][x]; // it should skip, but we need to copy
			}
		}
	}
	
	return change;
}

void printLand(Land land) {
	for (size_t y = 0; y < land.rows; y++) {
		for (size_t x = 0; x < land.columns; x++) printf("%c", land.land[y][x]);
		printf("\n");
	}
	printf("\n");
}

int main() {
	Land land = getLand(FILE_NAME), tmp;
	bool change;
	int cycle = 0;
	
	do {
		change = leftTurn(land, &tmp);
		freeLand(&land);
		change |= downTurn(tmp, &land);
		freeLand(&tmp);
		
		cycle++;
	} while (change);
	
	printLand(land);

	printf("%d\n", cycle);
	freeLand(&land);
}
