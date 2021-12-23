#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include <vector>
#include <cmath>        // std::abs
#include <algorithm>	// std::reverse

#define FILE_NAME 	"test.txt"//"input.txt"
#define ROOMS		4
#define ROOM_SIZE	2
#define HALLWAY_SIZE 11

typedef enum {
	A,
	B,
	C,
	D,
	None
} Amphipod;

class Hallway {
private:
	Amphipod position[HALLWAY_SIZE];

public:
	Hallway() {
		for (int x = 0; x < HALLWAY_SIZE; x++) this->position[x] = None;
	}
	
	Hallway(const Hallway &that) {
		for (int x = 0; x < HALLWAY_SIZE; x++) this->position[x] = that.position[x];
	}
	
	/**
	 * @param room	Habitació del amfípod (de 0 a ROOMS-1)
	 * @param row	Tamany del vector (de 1 [avall de tot] a 2 [amunt])
	 */
	static std::pair<int,int> absoluteToCoordinates(int room, int row) {
		return std::make_pair(2+(room*2), (row == 1) ? 2 : 1);
	}
	
	/**
	 * @param room	Habitació del amfípod (de 0 a ROOMS-1)
	 * @param row	Tamany del vector (de 1 [avall de tot] a 2 [amunt])
	 * @param pos_hallway	A on descansarà (0, 1, 3, 5, 7, 9, 10)
	 */
	int getCost(int room, int row, int pos_hallway) {
		std::pair<int,int> coord = Hallway::absoluteToCoordinates(room, row);
		
		// es pot arribar?
		// de esquerra a dreta
		for (int x = coord.first; x <= pos_hallway; x++) {
			if (this->position[x] != None) return INT_MAX; // hi ha un amfípod pel mitj -> no es pot arribar
		}
		// de dreta a esquerra
		for (int x = coord.first; x >= pos_hallway; x--) {
			if (this->position[x] != None) return INT_MAX; // hi ha un amfípod pel mitj -> no es pot arribar
		}
		
		return std::abs(pos_hallway - coord.first) + coord.second;
	}
	
	Hallway addAmphipod(int pos_hallway, Amphipod amphipod) {
		this->position[pos_hallway] = amphipod;
		return *this;
	}
	
	Hallway removeAmphipod(int pos_hallway) {
		this->position[pos_hallway] = None;
		return *this;
	}
	
	Amphipod getAmphipod(int pos_hallway) {
		return this->position[pos_hallway];
	}
};

void getInitialAmphipods(std::vector<Amphipod> room_amphipods[ROOMS], const char *file_name) {
	FILE *file = NULL;
	file = fopen(file_name, "r");
	
	char rooms[ROOMS];
	fscanf(file, "%*[^\n]%*c");
	fscanf(file, "%*[^\n]%*c");
	for (int x = 0; x < ROOM_SIZE; x++)	{
		fscanf(file, "%*3[ #]%c%*c%c%*c%c%*c%c%*4[# \n]", &rooms[0], &rooms[1], &rooms[2], &rooms[3]);
		for (int r = 0; r < ROOMS; r++) room_amphipods[r].push_back((Amphipod)(rooms[r]-'A'));
	}
	
	// s'han insertat del reves
	for (int r = 0; r < ROOMS; r++) std::reverse(room_amphipods[r].begin(), room_amphipods[r].end());
	fclose(file);
}

bool roomOK(std::vector<Amphipod> room_amphipods, Amphipod amphipods_room) {
	bool ok = true;
	while (!room_amphipods.empty() && ok) {
		ok = (room_amphipods.back() == amphipods_room);
		room_amphipods.pop_back();
	}
	return ok;
}

bool roomOK(std::vector<Amphipod> *room_amphipods, Amphipod amphipods_room) {
	return roomOK(room_amphipods[amphipods_room], amphipods_room);
}

bool roomOK(std::vector<Amphipod> room_amphipods, int amphipods_room) {
	return roomOK(room_amphipods, (Amphipod)amphipods_room);
}

bool roomOK(std::vector<Amphipod> *room_amphipods, int amphipods_room) {
	return roomOK(room_amphipods[amphipods_room], (Amphipod)amphipods_room);
}

int getAmphipodCost(Amphipod amphipod, int movements) {
	switch (amphipod) {
		case 0:
			return movements;
		
		case 1:
			return movements*10;
		
		case 2:
			return movements*100;
		
		case 3:
			return movements*1000;
			
		default:
			return 0;
	}
	//return movements * std::pow(10, (int)amphipod); // el 0 té cost 1, el 1 cost 10, el 2 100...
}

int iterate(std::vector<Amphipod> *room_amphipods, Hallway hallway, int total_cost, int max_cost) {
	int r = total_cost, cost;
	Amphipod tmp;
	bool valid;
	std::vector<Amphipod> cpy[ROOMS];
	
	// per cada un dels amfípods dels passadissos ha de decidir si pot anar a la seva habitació
	for (int x = 0; x < HALLWAY_SIZE; x++) {
		if ((tmp = hallway.getAmphipod(x)) == None) continue;
		
		if (!roomOK(room_amphipods, tmp)) continue; // a la habitació hi ha un amfípod d'un altre tipus que ha de ser eliminat abans
		
		for (int x = 0; x < ROOMS; x++) cpy[x] = room_amphipods[x];
		cpy[tmp].push_back(tmp); // l'amfípod s'afegeix a l'habitació
		cost = total_cost+getAmphipodCost(tmp, hallway.getCost(tmp, cpy[tmp].size(), x));
		if (cost < max_cost) {
			r = iterate(cpy, Hallway(hallway).removeAmphipod(x) /* l'amfípod s'elimina del passadis */, cost, r);
			max_cost = std::min(max_cost, r);
		}
	}
	
	// per cada un dels amfípods dels inicis de les habitacions incorrectes ha de decidir si va a la seva habitació, o al passadis
	for (int x = 0; x < ROOMS; x++) {
		if (room_amphipods[x].empty()) continue;
		
		valid = true;
		tmp = room_amphipods[x].back();
		if (roomOK(room_amphipods, x)) valid = false;
		else {
			// pot anar directament a la seva habitació?
			if (roomOK(room_amphipods, tmp) && (cost = hallway.getCost(2+(x*2), room_amphipods[x].size(), 2+(tmp*2))) < INT_MAX) {
				cost += total_cost /* & acumulat */ + Hallway::absoluteToCoordinates(tmp, room_amphipods[tmp].size()+1).second; // queda entrar a la sala
				
				for (int n = 0; n < ROOMS; n++) cpy[n] = room_amphipods[n];
				cpy[tmp].push_back(tmp);
				cpy[x].pop_back();
				if (cost < max_cost) {
					r = iterate(cpy, hallway, cost, r);
					max_cost = std::min(max_cost, r);
				}
			}
			else valid = false;
		}
		
		if (!valid) {
			// ha d'anar al passadis
			for (int y = 0; y < HALLWAY_SIZE; y++) {
				for (int n = 0; n < ROOMS; n++) cpy[n] = room_amphipods[n];
				cpy[x].pop_back();
				cost = total_cost+getAmphipodCost(tmp, hallway.getCost(2+(x*2), room_amphipods[x].size(), y));
				if (cost < max_cost) {
					r = iterate(cpy, Hallway(hallway).addAmphipod(y, tmp), cost, r);
					max_cost = std::min(max_cost, r);
				}
			}
		}
	}
	
	return r;
}

int main() {
	std::vector<Amphipod> room_amphipods[ROOMS];
	getInitialAmphipods(room_amphipods, FILE_NAME);
	
	printf("%d\n", iterate(room_amphipods, Hallway(), 0, INT_MAX));
	
	return 0;
}
