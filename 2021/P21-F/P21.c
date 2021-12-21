#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME 	"test.txt"//"input.txt"
#define PLAYERS		2
#define ROLL_DICE	100

typedef struct {
	unsigned int score;
	unsigned char position;
} Player;

unsigned int dice_throws = 0;
unsigned int last_value = 0;
unsigned char next = 0;
unsigned char getDiceValue() {
	unsigned int current;
	unsigned char r;
	
	dice_throws++;
	next += 3;
	
	if (next > ROLL_DICE) {
		r = ((next-3) % 100)+1;
		r += ((next-2) % 100)+1;
		current = ((next-1) % 100)+1;
		r += current;
		
		//printf("%d + %d + %d: %d\n", ((next-3) % 100)+1, ((next-2) % 100)+1, ((next-1) % 100)+1, r);
		
		last_value = current*(current+1)/2;
		next %= 100;
	}
	else {
		current = next*(next + 1)/2;
		r = current - last_value;
		//printf("%d + %d + %d: %d (%d-%d)\n", next-2, next-1, next, r, current, last_value);
		
		last_value = current;
	}
	return r;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	Player players[PLAYERS] = {0};
	
	for (int x = 0; x < PLAYERS; x++) {
		fscanf(file, "%*[^:]: %hhu%*c", &players[x].position);
	}
	
	int turno = -1;
	unsigned char new_pos;
	do {
		turno = (turno+1)%2;
		
		players[turno].position = ((players[turno].position+getDiceValue()-1)%10)+1;
		players[turno].score += players[turno].position;
		printf("%d: %d, %d\n", turno, players[turno].position, players[turno].score);
	} while (players[turno].score < 1000);

	printf("%llu\n", (unsigned long long)players[!turno].score*dice_throws);
	fclose(file);
}
