#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <set>
#include <vector>
#include <algorithm>

#define FILE_NAME 	"input.txt"
#define PLAYERS		2
#define ROLLS		3
#define ROLL_DICE	3
#define SCORE		21
#define POSITION_ROTATION 10

typedef std::vector<unsigned long long> Wins;

class Player {
private:
public:
	unsigned char score;
	unsigned char position;
	
	Player() {
		this->score = 0;
		this->position = 0;
	}
	
	Player(unsigned char score, unsigned char position) {
		this->score = score;
		this->position = position;
	}
	
	bool operator==(const Player& that) const {
		return this->score == that.score && this->position == that.position;
	}
};

class Instance {
private:
public:
	Player player1, player2;
	unsigned long long players_wins[PLAYERS];
	bool player1_turn;
	unsigned int id;
	
	unsigned int getID() const {
		// simetric (Instance(p1,p2) == Instance(p2,p1))
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
	
	Instance(Player player1, unsigned long long player1_score, Player player2, unsigned long long player2_score, bool player1_turn) {
		this->player1 = player1;
		this->players_wins[0] = player1_score;
		this->player2 = player2;
		this->players_wins[1] = player2_score;
		this->player1_turn = player1_turn;
		
		this->id = getID();
	}
	
	Player getPlayer1() const {
		return this->player1;
	}
	
	unsigned long long getPlayer1Wins() const {
		return this->players_wins[0];
	}
	
	unsigned long long getPlayer2Wins() const {
		return this->players_wins[1];
	}
	
	bool operator<(const Instance& that) const {
		//return this->id < that.id;
		if (this->player1.score != that.player1.score) return this->player1.score < that.player1.score;
		if (this->player1.position != that.player1.position) return this->player1.position < that.player1.position;
		if (this->player2.score != that.player2.score) return this->player2.score < that.player2.score;
		if (this->player2.position != that.player2.position) return this->player2.position < that.player2.position;
		return this->player1_turn < that.player1_turn;
	}
};

std::set<Instance> instances;

Wins getWins(Player player1, Player player2, bool player1_turn) {
	Wins r;
	std::set<Instance>::iterator iter = instances.find(Instance(player1, player2, player1_turn));
	if (iter == instances.end()) return r;
	
	if (iter->getPlayer1() == player1) {
		r.push_back(iter->getPlayer1Wins());
		r.push_back(iter->getPlayer2Wins());
	}
	else {
		r.push_back(iter->getPlayer2Wins());
		r.push_back(iter->getPlayer1Wins());
	}
	return r;
}

void addWin(Player player1, Player player2, bool player1_turn, Wins wins) {
	instances.insert(Instance(player1, wins[0], player2, wins[1], player1_turn));
}

Wins computeDice(Player player1, Player player2, bool player1_turn) {
	Player p;
	Wins wins(PLAYERS, 0), cache;
	
	for (int x1 = 1; x1 <= ROLL_DICE; x1++) {
		for (int x2 = 1; x2 <= ROLL_DICE; x2++) {
			for (int x3 = 1; x3 <= ROLL_DICE; x3++) {
				p = (player1_turn ? player1 : player2);
				p.position = (p.position+x1+x2+x3-1)%POSITION_ROTATION + 1;
				p.score += p.position;
				
				if (p.score >= SCORE) {
					wins[!player1_turn]++; // si player1_turn ha de ser 0, si no ha de ser 1
					//addWin((player1_turn ? p : player1), (player1_turn ? player2 : p), player1_turn, );
				}
				else {
					// s'ha de seguir amb el següent
					cache = getWins((player1_turn ? p : player1), (player1_turn ? player2 : p), !player1_turn);
					if (cache.empty()) {
						// no s'ha trobat
						cache = computeDice((player1_turn ? p : player1), (player1_turn ? player2 : p), !player1_turn);
						
						addWin((player1_turn ? p : player1), (player1_turn ? player2 : p), !player1_turn, cache);
					}
					
					// afegeix el resultat al llistat de victories
					std::transform(wins.begin(), wins.end(), cache.begin(), wins.begin(), std::plus<unsigned long long>());
				}
			}
		}
	}
	
	return wins;
}

int main() {
	FILE *file = NULL;
	Player players[PLAYERS];
	
	unsigned char pos;
	file = fopen(FILE_NAME, "r");
	for (int x = 0; x < PLAYERS; x++) {
		fscanf(file, "%*[^:]: %hhu%*c", &pos);
		players[x] = Player(0, pos);
	}
	fclose(file);
	
	Wins w = computeDice(players[0], players[1], true);

	printf("%llu\n", *std::max_element(w.begin(), w.end()));
	
	return 0;
}
