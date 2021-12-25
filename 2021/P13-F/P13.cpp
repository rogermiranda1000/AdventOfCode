#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <stdint.h>

#define FILE_NAME "input.txt"

typedef struct {
	int fold_x, fold_y;
} Instruction;

typedef struct {
	int x, y;
} Dot;

#define MAP_VAR_TYPE bool

typedef struct {
	MAP_VAR_TYPE **map;
	int x_size, y_size;
} Map;

void allocateMap(Map *map) {
	map->map = (MAP_VAR_TYPE**)malloc(map->y_size*sizeof(MAP_VAR_TYPE*));
	for (int y = 0; y < map->y_size; y++) {
		map->map[y] = (MAP_VAR_TYPE*)malloc(map->x_size*sizeof(MAP_VAR_TYPE));
		for (int x = 0; x < map->x_size; x++) map->map[y][x] = false;
	}
}

Map readMap(Instruction **instructions, size_t *instructions_size, const char *file_name) {
	FILE *file = NULL;
	file = fopen(file_name, "r");
	
	std::set< std::pair <int,int> > data;
	std::pair <int,int> dot, max_dot = std::make_pair(0,0);
	char buffer[100];
	fscanf(file, "%[0-9\n]", buffer);
	while (*buffer != '\n') {
		dot.first = atoi(buffer);
		if (dot.first > max_dot.first) max_dot.first = dot.first;
		fscanf(file, ",%d%*c", &dot.second);
		if (dot.second > max_dot.second) max_dot.second = dot.second;
		
		data.insert(dot);
		
		fscanf(file, "%[0-9\n]", buffer);
	}
	
	char var;
	int tmp;
	*instructions_size = 0;
	*instructions = NULL;
	fscanf(file, "fold along %c=%d%*c", &var, &tmp);
	while (!feof(file)) {
		(*instructions_size)++;
		*instructions = (Instruction*)realloc(*instructions, (*instructions_size)*sizeof(Instruction));
		if (var == 'x') (*instructions)[*instructions_size - 1] = (Instruction){tmp, -1};
		else (*instructions)[*instructions_size - 1] = (Instruction){-1, tmp};
		
		fscanf(file, "fold along %c=%d%*c", &var, &tmp);
	}
	
	fclose(file);
	
	Map r = (Map){NULL, max_dot.first+1, max_dot.second+1};
	allocateMap(&r);
	
	for (int y = 0; y < r.y_size; y++) {
		for (int x = 0; x < r.x_size; x++) {
			r.map[y][x] = (data.find(std::make_pair(x, y)) != data.end());
		}
	}
	
	return r;
}

void freeMap(Map *map) {
	for (int y = 0; y < map->y_size; y++) free(map->map[y]);
	free(map->map);
	
	map->map = NULL;
	map->x_size = 0;
	map->y_size = 0;
}

void printMap(Map map) {
	for (int y = 0; y < map.y_size; y++) {
		for (int x = 0; x < map.x_size; x++) {
			printf("%c", map.map[y][x] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\n");
}

Map fold(Map map, Instruction instruction) {
	Map new_map;
	if (instruction.fold_x != -1) {
		// x-fold
		new_map = (Map){NULL, 0, map.y_size};
		if (instruction.fold_x >= map.x_size-instruction.fold_x-1) {
			// the upper part is bigger
			new_map.x_size = instruction.fold_x;
			allocateMap(&new_map);
			
			// copy the previous map
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[y][x] |= map.map[y][x];
			}
			
			// mirror the other part
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = new_map.x_size+1, current = new_map.x_size-1; x < map.x_size; x++, current--) new_map.map[y][current] |= map.map[y][x];
			}
		}
		else {
			// the bottom part is bigger
			new_map.x_size = map.x_size-instruction.fold_x-1;
			allocateMap(&new_map);
			
			// copy the previous map
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[y][x] |= map.map[y][map.x_size-1-x];
			}
			
			// mirror the other part
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = 0; x < instruction.fold_x; x++) new_map.map[y][new_map.x_size-instruction.fold_x+x] |= map.map[y][x];
			}
		}
	}
	else {
		// y-fold
		new_map = (Map){NULL, map.x_size, 0};
		if (instruction.fold_y >= map.y_size-instruction.fold_y-1) {
			// the upper part is bigger
			new_map.y_size = instruction.fold_y;
			allocateMap(&new_map);
			
			// copy the previous map
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[y][x] |= map.map[y][x];
			}
			
			// mirror the other part
			for (int y = new_map.y_size+1, current = new_map.y_size-1; y < map.y_size; y++, current--) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[current][x] |= map.map[y][x];
			}
		}
		else {
			// the bottom part is bigger
			new_map.y_size = map.y_size-instruction.fold_y-1;
			allocateMap(&new_map);
			
			// copy the previous map
			for (int y = 0; y < new_map.y_size; y++) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[y][x] |= map.map[map.y_size-1-y][x];
			}
			
			// mirror the other part
			for (int y = 0; y < instruction.fold_y; y++) {
				for (int x = 0; x < new_map.x_size; x++) new_map.map[new_map.y_size-instruction.fold_y+y][x] |= map.map[y][x];
			}
		}
	}
	return new_map;
}

int countDots(Map map) {
	int count = 0;
	for (int y = 0; y < map.y_size; y++) {
		for (int x = 0; x < map.x_size; x++) count += map.map[y][x];
	}
	return count;
}

int main() {
	Instruction *instructions;
	size_t instructions_size;
	Map map = readMap(&instructions, &instructions_size, FILE_NAME), tmp;
	
	//printMap(map);
	//for (int x = 0; x < instructions_size; x++) {
		int x = 0;
		tmp = fold(map, instructions[x]);
		freeMap(&map);
		map = tmp;
		
		//printMap(map);
	//}

	printf("%d\n", countDots(map));
	
	free(instructions);
	freeMap(&map);
	return 0;
}
