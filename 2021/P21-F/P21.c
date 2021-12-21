#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME 	"input.txt"
#define PLAYERS		2
#define ROLL_DICE	100

typedef struct {
	unsigned int score;
	unsigned char position;
} Player;

unsigned int dice_throws = 0;
unsigned int last_value = 0; // 5050 worst-case
unsigned char next = 0;
unsigned int getDiceValue() {
	unsigned int current;
	unsigned int r; // 297 worst-case
	
	dice_throws += 3;
	next += 3;
	
	//printf("[%d] ", dice_throws);
	if (next > ROLL_DICE) {
		r = ((next-3) % ROLL_DICE)+1;
		r += ((next-2) % ROLL_DICE)+1;
		current = ((next-1) % ROLL_DICE)+1; // aux
		r += current;
		
		//printf("%d + %d + %d: %d\n", ((next-3) % ROLL_DICE)+1, ((next-2) % ROLL_DICE)+1, ((next-1) % ROLL_DICE)+1, r);
		
		last_value = current*(current+1)/2;
		next -= ROLL_DICE; // en aquest cas, equivalent a rext%=ROLL_DICE
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
	Player players[PLAYERS] = {0};
	
	file = fopen(FILE_NAME, "r");
	for (int x = 0; x < PLAYERS; x++) fscanf(file, "%*[^:]: %hhu%*c", &players[x].position);
	fclose(file);
	
	int turno = -1;
	do {
		turno = (turno+1)%2;
		
		players[turno].position = ((players[turno].position+getDiceValue()-1)%10)+1;
		players[turno].score += players[turno].position;
		//printf("%d: %d, %d\n", turno, players[turno].position, players[turno].score);
	} while (players[turno].score < 1000);

	printf("%llu\n", (unsigned long long)players[!turno].score*dice_throws);
	
	return 0;
}
