#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utility>      // std::pair
#include <set>
#include <limits.h>
#include <unistd.h>

#define ENHANCEMENT_LEN	512
#define FILE_NAME 		"input.txt"

#define ENHANCEMENTS	50

typedef std::set< std::pair <int,int> > Pixels;

typedef struct {
	Pixels white;
	Pixels black;
	bool unchecked_are_black;
} Enhancement;

bool isBright(std::pair <int,int> dot, Pixels *white, Pixels *black, bool unchecked_are_black, char *enhancement) {
	short data = 0;
	for (int y = dot.second-1; y <= dot.second+1; y++) {
		for (int x = dot.first-1; x <= dot.first+1; x++) {
			data *= 2;
			if (white->find( std::make_pair(x, y) ) != white->end() // s'ha trobat pixel blanc
				|| (black->find( std::make_pair(x, y) ) == black->end() && !unchecked_are_black)) data++; // no s'ha trobat pixel negre, i els desconeguts son blancs
		}
	}
	
	return (enhancement[data] == '#');
}

Enhancement enhance(Enhancement img, char *enhancement) {
	Enhancement r;
	std::pair <int,int> tmp;
	
	if (img.unchecked_are_black) r.unchecked_are_black = (enhancement[0] == '.');
	else r.unchecked_are_black = (enhancement[ENHANCEMENT_LEN-1] == '.');
	
	// s'han de procesar els punts blancs i negres, i els del seu costat
	Pixels *vector = &img.white;
	if (vector->empty()) vector = &img.black; // només 1 dels 2 tindrá dades
	for(auto e : *vector) {
		for (int y = e.second-1; y <= e.second+1; y++) {
			for (int x = e.first-1; x <= e.first+1; x++) {
				tmp = std::make_pair(x, y);
				if (isBright(tmp, &img.white, &img.black, img.unchecked_are_black, enhancement)) {
					if (r.unchecked_are_black) r.white.insert(tmp); // si els que no estan marcats son negres, s'han d'afegir els punts blancs
				}
				else {
					if (!r.unchecked_are_black) r.black.insert(tmp); // si els que no estan marcats son blancs, s'han d'afegir els punts negres
				}
			}
		}
	}
	
	return r;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char enhancement[ENHANCEMENT_LEN+2];
	Enhancement img = {Pixels(), Pixels(), true};
	
	fgets(enhancement, ENHANCEMENT_LEN+2, file);
	enhancement[strlen(enhancement)-1] = '\0';
	
	fscanf(file, "%*c"); // eliminem la linea de separació
	
	char tmp;
	int x = 0, y = 0;
	fscanf(file, "%c", &tmp);
	while (!feof(file)) {
		if (tmp == '\n') {
			x = 0;
			y++;
		}
		else {
			if (tmp == '#') img.white.insert( std::make_pair(x, y) );
			x++;
		}
		
		fscanf(file, "%c", &tmp);
	}
	
	for (int x = 0; x < ENHANCEMENTS; x++) img = enhance(img, enhancement);
	
	if (!img.unchecked_are_black) {
		printf("[e] Inf\n");
		exit(EXIT_FAILURE);
	}

	printf("%ld\n", img.white.size());
	fclose(file);
}
