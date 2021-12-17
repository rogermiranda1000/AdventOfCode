#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int x, y;
	short occurrencies;
} Dot;

Dot *getDot(Dot **dots, size_t *dot_size, int x, int y) {
	for (size_t n = 0; n < *dot_size; n++) {
		if ((*dots)[n].x == x && (*dots)[n].y == y) return &(*dots)[n];
	}
	
	// no trobat, afegir
	(*dot_size)++;
	*dots = (Dot*)realloc(*dots, sizeof(Dot)*(*dot_size));
	if (*dots == NULL) {
		printf("Malloc error!\n");
		exit(EXIT_FAILURE);
	}
	
	(*dots)[*dot_size - 1] = (Dot){x, y, 0};
	return &(*dots)[*dot_size - 1];
}

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	Dot *dots = NULL;
	size_t dot_size = 0;
	int x1, y1, x2, y2, tmp;
	char instruction;
	int num;
	fscanf(file, "%d,%d -> %d,%d%*c", &x1, &y1, &x2, &y2);
	while(!feof(file)) {
		// x1,y1 han de ser menor a x2,y2
		if (x1 > x2) {
			tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		if (y1 > y2) {
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		
		// afegir la linea
		bool diagonal = (x1 != x2 && y1 != y2);
		for (int x = x1; x <= x2; x+=1+diagonal) {
			for (int y = y1; y <= y2; y+=1+diagonal) {
				getDot(&dots, &dot_size, x, y)->occurrencies++;
			}
		}
		
		fscanf(file, "%d,%d -> %d,%d%*c", &x1, &y1, &x2, &y2);
	}
	
	int matches = 0;
	for (size_t n = 0; n < dot_size; n++) {
		if (dots[n].occurrencies > 1) matches++;
	}

	printf("%d\n", matches);
	free(dots);
	fclose(file);
}
