#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <set>

#define FILE_NAME 	"test.txt"//"input.txt"
#define PLAYERS		2
#define ROLL_DICE	3
#define TROWS		3
#define SCORE		21
#define POSITION_ROTATION 10

typedef struct {
	unsigned char score;
	unsigned char position;
	unsigned long long wins;
} Player;

class Instance {
private:
public:
	Player player1, player2;
	bool player1_turn;
	unsigned int id;
	
	unsigned int getID() const {
		if (player1.score <= player2.score) {
			return ((unsigned int)player1_turn<<31) |
				((unsigned int)this->player1.position<<28) | ((unsigned int)this->player1.score<<16) |
				((unsigned short)this->player2.position<<8) | this->player2.score;
		}
		// else
		return ((unsigned int)(!player1_turn)<<31) |
			((unsigned int)this->player2.position<<28) | ((unsigned int)this->player2.score<<16) |
			((unsigned short)this->player1.position<<8) | this->player1.score;
	}
	
	Instance(Player player1, Player player2, bool player1_turn) {
		this->player1 = player1;
		this->player2 = player2;
		this->player1_turn = player1_turn;
		
		this->id = getID();
	}
	
	bool isPlayer1(Player p) const {
		return p.score == this->player1.score && p.position == this->player1.position;
	}
	
	unsigned long long getPlayer1Wins() const {
		return this->player1.wins;
	}
	
	unsigned long long getPlayer2Wins() const {
		return this->player2.wins;
	}
	
	bool operator<(const Instance& that) const  {
		return this->id < that.id;
	}
};

std::set<Instance> instances;

bool getWins(Player *player1, Player *player2, bool player1_turn) {
	std::set<Instance>::iterator iter = instances.find(Instance(*player1, *player2, player1_turn));
	if (iter == instances.end()) return false;
	
	if (iter->isPlayer1(*player1)) {
		player1->wins += iter->getPlayer1Wins();
		player2->wins += iter->getPlayer2Wins();
	}
	else {
		player1->wins += iter->getPlayer2Wins();
		player2->wins += iter->getPlayer1Wins();
	}
	return true;
}

void addWin(Player player1, Player player2, bool player1_turn) {
	instances.insert(Instance(player1, player2, player1_turn));
}

void computeDice(Player *player1, Player *player2, bool player1_turn) {
	Player p, pother;
	unsigned long long p1_wins = 0, p2_wins = 0;
	
	for (int x1 = 1; x1 <= ROLL_DICE; x1++) {
		for (int x2 = 1; x2 <= ROLL_DICE; x2++) {
			for (int x3 = 1; x3 <= ROLL_DICE; x3++) {
				if (player1_turn) {
					p = *player1;
					p.position = (p.position+x1+x2+x3-1)%POSITION_ROTATION + 1;
					p.score += p.position;
					
					pother = *player2;
					
					if (p.score >= SCORE) {
						p.wins++;
						//addWin(p, pother, false);
					}
					else {
						if (!getWins(&p, &pother, false)) {
							computeDice(&p, &pother, false);
							addWin(p, pother, false);
						}
					}
					
					p1_wins += p.wins - player1->wins;
					p2_wins += pother.wins - player2->wins;
				}
				else {
					p = *player2;
					p.position = (p.position+x1+x2+x3-1)%POSITION_ROTATION + 1;
					p.score += p.position;
					
					pother = *player1;
					
					if (p.score >= SCORE) {
						p.wins++;
						//addWin(pother, p, true);
					}
					else {
						if (!getWins(&pother, &p, true)) {
							computeDice(&pother, &p, true);
							addWin(pother, p, true);
						}
					}
					
					p1_wins += pother.wins - player1->wins;
					p2_wins += p.wins - player2->wins;
				}
			}
		}
	}
	
	player1->wins += p1_wins;
	player2->wins += p2_wins;
}

int main() {
	FILE *file = NULL;
	Player players[PLAYERS] = {0};
	
	file = fopen(FILE_NAME, "r");
	for (int x = 0; x < PLAYERS; x++) fscanf(file, "%*[^:]: %hhu%*c", &players[x].position);
	fclose(file);
	
	computeDice(&players[0], &players[1], true);
	
	/*for (Instance const& instance : instances) {
		printf("%d, %d: %lld -> %d, %d: %lld [%d]\n", instance.player1.position, instance.player1.score, instance.getPlayer1Wins(),
													instance.player2.position, instance.player2.score, instance.getPlayer2Wins(),
													instance.player1_turn);
	}*/

	printf("%llu\n", (players[0].wins > players[1].wins) ? players[0].wins : players[1].wins);
	
	return 0;
}
