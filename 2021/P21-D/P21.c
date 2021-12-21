#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME 	"test.txt"//"input.txt"
#define PLAYERS		2
#define ROLL_DICE	3
#define TROWS		3
#define SCORE		21
#define POSITION_ROTATION 10

typedef struct {
	unsigned int score;
	unsigned char position;
	unsigned long long wins;
} Player;

#define compute(player1, player2, turno) (computeDice(player1, player2, turno, 0, 0))

void computeDice(Player *player1, Player *player2, int turno, int dice_trow, int dice_sum) {
	if (dice_trow < TROWS) {
		for (int x = 1; x <= ROLL_DICE; x++) computeDice(player1, player2, turno, dice_trow+1, dice_sum+x);
	}
	else {
		Player p;
		if (turno == 0) {
			p = *player1;
			p.position = (p.position+dice_sum-1)%POSITION_ROTATION + 1;
			p.score += p.position;
			
			if (p.score >= SCORE) p.wins++;
			else compute(&p, player2, 1);
			
			player1->wins = p.wins;
		}
		else {
			p = *player2;
			p.position = (p.position+dice_sum-1)%POSITION_ROTATION + 1;
			p.score += p.position;
			
			if (p.score >= SCORE) p.wins++;
			else compute(player1, &p, 0);
			
			player2->wins = p.wins;
		}
	}
}

int main() {
	FILE *file = NULL;
	Player players[PLAYERS] = {0};
	
	file = fopen(FILE_NAME, "r");
	for (int x = 0; x < PLAYERS; x++) fscanf(file, "%*[^:]: %hhu%*c", &players[x].position);
	fclose(file);
	
	compute(&players[0], &players[1], 0);

	printf("%llu\n", (players[0].wins > players[1].wins) ? players[0].wins : players[1].wins);
	
	return 0;
}
