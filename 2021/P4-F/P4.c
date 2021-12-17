#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CARBOARD_COLUMNS 5
#define CARBOARD_ROWS 5

#define FILE_NAME "input.txt"

typedef struct {
	int number;
	bool marked;
} Entry;

typedef Entry Carboard[CARBOARD_ROWS][CARBOARD_COLUMNS];

bool searchIndex(Carboard *c, int searching, int *row, int *column) {
	for (int a = 0; a < CARBOARD_ROWS; a++) {
		for (int b = 0; b < CARBOARD_COLUMNS; b++) {
			if ((*c)[a][b].number == searching) {
				*row = a;
				*column = b;
				return true;
			}
		}
	}
	return false;
}

void markIfFound(Carboard *c, int searching) {
	int x, y;
	if (searchIndex(c, searching, &x, &y)) (*c)[x][y].marked = true;
}

bool haveBingo(Carboard *c) {
	bool all;
	for (int x = 0; x < CARBOARD_ROWS; x++) {
		all = true;
		for (int y = 0; y < CARBOARD_COLUMNS && all; y++) {
			all = (*c)[x][y].marked;
		}
		if (all) return true;
	}
	
	for (int y = 0; y < CARBOARD_COLUMNS; y++) {
		all = true;
		for (int x = 0; x < CARBOARD_ROWS && all; x++) {
			all = (*c)[x][y].marked;
		}
		if (all) return true;
	}
	
	return false;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char buffer[10];
	size_t num_list_size = 0;
	int aux, *num_list = NULL;
	
	do {
		fscanf(file, "%d%c", &aux, buffer);
		num_list = (int*)realloc(num_list, sizeof(int)*(++num_list_size));
		num_list[num_list_size-1] = aux;
	} while (*buffer == ',');
	
	fscanf(file, "%*c"); // espai carboards
	
	size_t carboard_size = 0;
	Carboard *carboards = NULL;
	int row, column;
	while(!feof(file)) {
		carboards = realloc(carboards, sizeof(Carboard)*(++carboard_size));
		
		row = 0;
		do {
			column = 0;
			do {
				fscanf(file, "%d%[^0-9]", &aux, buffer);
				carboards[carboard_size-1][row][column++] = (Entry){aux, false};
			} while (*buffer == ' ');
			row++;
		} while (row < CARBOARD_ROWS);
		
		// espai carboards borrat pel "regex" del fscanf
	}
	
	/*int carboard = 1;
	for (int x = 0; x < 5; x++) {
		printf("%d %d %d %d %d\n", carboards[carboard][x][0].number, carboards[carboard][x][1].number,
			carboards[carboard][x][2].number, carboards[carboard][x][3].number, carboards[carboard][x][4].number);
	}*/
	
	for (size_t num_index = 0; num_index < num_list_size; num_index++) {
		int number = num_list[num_index];
		for (size_t carboard_index = 0; carboard_index < carboard_size; carboard_index++) {
			Carboard *current = &carboards[carboard_index];
			markIfFound(current, number);
			if (haveBingo(current)) {
				int unmarked_sum = 0;
				for (int x = 0; x < CARBOARD_ROWS; x++) {
					for (int y = 0; y < CARBOARD_COLUMNS; y++) {
						if (!(*current)[x][y].marked) unmarked_sum += (*current)[x][y].number;
					}
				}
				
				printf("%d * %d\n", unmarked_sum, number);
				printf("%llu\n", ((unsigned long long)unmarked_sum)*number);
				goto end;
			}
		}
	}

end:
	//printf("%d\n", x*y);
	fclose(file);
	free(num_list);
	free(carboards);
}
