#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include <vector>
#include <cmath>        // std::abs
#include <algorithm>	// std::reverse
#include <tuple>

#define FILE_NAME 	"test2.txt"//"input.txt"
#define ROOMS		4
#define ROOM_SIZE	2
#define HALLWAY_SIZE 11
#define DESTINATION_UNREACHABLE -1

#define DEBUG

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
	
	static bool isInFrontOfRoom(int x) {
		return x == 2 || x == 4 || x == 6 || x == 8;
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
			if (this->position[x] != None) return DESTINATION_UNREACHABLE; // hi ha un amfípod pel mitj -> no es pot arribar
		}
		// de dreta a esquerra
		for (int x = coord.first-1; x >= pos_hallway; x--) {
			if (this->position[x] != None) return DESTINATION_UNREACHABLE; // hi ha un amfípod pel mitj -> no es pot arribar
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
	
	bool empty() {
		bool empty = true;
		for (int x = 0; x < HALLWAY_SIZE && empty; x++) empty = (this->position[x] == None);
		return empty;
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
		case A:
			return movements;
		
		case B:
			return movements*10;
		
		case C:
			return movements*100;
		
		case D:
			return movements*1000;
			
		default:
			return 0;
	}
	//return movements * std::pow(10, (int)amphipod); // el 0 té cost 1, el 1 cost 10, el 2 100...
}

void print(std::vector<Amphipod> *room_amphipods, Hallway hallway) {
	for (int x = 0; x < HALLWAY_SIZE; x++) printf("%c", hallway.getAmphipod(x) == None ? '.' : (hallway.getAmphipod(x)+'A'));
	printf("\n##");
	for (int x = 0; x < ROOMS; x++) {
		if (room_amphipods[x].size() > 1) printf("%c#", room_amphipods[x].back() + 'A');
		else printf(".#");
	}
	printf("#\n##");
	for (int x = 0; x < ROOMS; x++) {
		if (room_amphipods[x].size() > 0) printf("%c#", room_amphipods[x].front() + 'A');
		else printf(".#");
	}
	printf("#\n\n");
}

#define copyAmphipodArray(cpy, origen) (std::copy(origen, origen+ROOMS, cpy))

unsigned long long iterate(std::vector<Amphipod> *room_amphipods, Hallway hallway, std::vector<std::tuple<std::vector<Amphipod>*,Hallway,unsigned long long>> logs, unsigned long long total_cost, unsigned long long max_cost) {
	unsigned long long cost;
	Amphipod tmp;
	std::vector<Amphipod> cpy[ROOMS];
	
#ifdef DEBUG
	logs.push_back(std::tuple<std::vector<Amphipod>*,Hallway,unsigned long long>(room_amphipods, hallway, total_cost));
#endif
	
	// ja ha acabat?
	bool valid = hallway.empty();
	for (int x = 0; x < ROOMS && valid; x++) valid = roomOK(room_amphipods, x);
	if (valid) {
#ifdef DEBUG
		printf("Found in %llu\n", total_cost);
		for (auto &p : logs) {
			printf("%llu\n", std::get<2>(p));
			print(std::get<0>(p), std::get<1>(p));
		}
		printf("\n\n");
#endif
		return total_cost;
	}
	
	// per cada un dels amfípods dels passadissos ha de decidir si pot anar a la seva habitació
	for (int x = 0; x < HALLWAY_SIZE; x++) {
		if ((tmp = hallway.getAmphipod(x)) == None) continue;
		
		if (!roomOK(room_amphipods, tmp)) continue; // a la habitació hi ha un amfípod d'un altre tipus que ha de ser eliminat abans
		
		Hallway aux = Hallway(hallway).removeAmphipod(x); // l'amfípod s'elimina del passadis
		if ((cost = aux.getCost(tmp, room_amphipods[tmp].size()+1, x)) != DESTINATION_UNREACHABLE) {
			copyAmphipodArray(cpy, room_amphipods);
			cpy[tmp].push_back(tmp); // l'amfípod s'afegeix a l'habitació
			cost = total_cost+getAmphipodCost(tmp, cost);
			if (cost < max_cost) max_cost = std::min(max_cost, iterate(cpy, aux, logs, cost, max_cost));
		}
#ifdef DEBUG
		else printf("No puc moure %d!\n", x);
#endif
	}
	
	// per cada un dels amfípods dels inicis de les habitacions incorrectes ha de decidir si va a la seva habitació, o al passadis
	for (int x = 0; x < ROOMS; x++) {
		if (room_amphipods[x].empty()) continue;
		
		valid = true;
		tmp = room_amphipods[x].back();
		if (x != tmp && roomOK(room_amphipods, tmp)) {
			// la habitació destí està ordenada -> pot fer el salt directe
			// pot anar directament a la seva habitació?
			std::pair<int,int> aux = Hallway::absoluteToCoordinates(tmp, room_amphipods[tmp].size()+1);
			if ((cost = hallway.getCost(x, room_amphipods[x].size(), aux.first)) != DESTINATION_UNREACHABLE) {
				cost = total_cost + getAmphipodCost(tmp, cost) + aux.second /* queda entrar a la sala */;
				
				copyAmphipodArray(cpy, room_amphipods);
				cpy[tmp].push_back(tmp);
				cpy[x].pop_back();
				if (cost < max_cost) max_cost = std::min(max_cost, iterate(cpy, hallway, logs, cost, max_cost));
			}
			else valid = false;
		}
		else if (!roomOK(room_amphipods, x)) valid = false; // la habitació no està ordenada -> ha d'anar al passadis
		
		if (!valid) {
			// ha d'anar al passadis
			for (int y = 0; y < HALLWAY_SIZE; y++) {
				if (Hallway::isInFrontOfRoom(y)) continue; // no es poden posar allà
				
				if ((cost = hallway.getCost(x, room_amphipods[x].size(), y)) != DESTINATION_UNREACHABLE) {
					copyAmphipodArray(cpy, room_amphipods);
					cpy[x].pop_back();
					cost = total_cost+getAmphipodCost(tmp, cost);
					if (cost < max_cost) max_cost = std::min(max_cost, iterate(cpy, Hallway(hallway).addAmphipod(y, tmp), logs, cost, max_cost));
				}
			}
		}
	}
	
	return max_cost;
}

int main() {
	std::vector<Amphipod> room_amphipods[ROOMS];
	getInitialAmphipods(room_amphipods, FILE_NAME);
	
	printf("%llu\n", iterate(room_amphipods, Hallway(), std::vector<std::tuple<std::vector<Amphipod>*,Hallway,unsigned long long>>(), 0, INT_MAX));
	
	return 0;
}
