#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define FILE_NAME	"input.txt"
#define DAY			256

#define RESET_TIME 		6 // 7 cicles (de 6 a 0)
#define TIME_PENALTY 	2 // els primers començen amb 8 (RESET_TIME+2)
#define FISHES_SIZE		(RESET_TIME+TIME_PENALTY+1)

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	clock_t start = clock(), end;
	unsigned long aux;
	unsigned long fishes[FISHES_SIZE] = {0};
	
	fscanf(file, "%ld%*c", &aux);
	while(!feof(file)) {
		fishes[aux]++;
		
		fscanf(file, "%ld%*c", &aux);
	}
	
	for (int x = 0; x < DAY; x++) {
		aux = fishes[0];
		// movem els peixos al anterior estat
		for (int x = 1; x < FISHES_SIZE; x++) {
			fishes[x-1] = fishes[x];
		}
		// (ja ees sobreescriu abaix)
		//fishes[FISHES_SIZE-1] = 0;
		
		// els que quedaven 0 dies, es reprodueixen
		fishes[RESET_TIME] += aux; // els que s'han reproduit passen a RESET_TIME
		fishes[RESET_TIME+TIME_PENALTY] = aux; // els nous tenen la penalització
	}
	
	// count fishes
	aux = 0;
	for (int x = 0; x < FISHES_SIZE; x++) aux += fishes[x];
	end = clock();
	
	printf("%ld\n", aux);
	printf("Found on %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	
	fclose(file);
}
