#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utility>      // std::pair
#include <set>
#include <limits.h>
#include <unistd.h>

#define ENHANCEMENT_LEN	512
#define FILE_NAME 		"test.txt"//"input.txt"

bool isBright(std::pair <int,int> dot, std::set< std::pair <int,int> > *img, char *enhancement) {
	short data = 0;
	for (int y = dot.second-1; y <= dot.second+1; y++) {
		for (int x = dot.first-1; x <= dot.first+1; x++) {
			data *= 2;
			data += (img->find( std::make_pair(x, y) ) != img->end());
		}
	}
	
	return (enhancement[data] == '#');
}

std::set< std::pair <int,int> > enhance(std::set< std::pair <int,int> > img, char *enhancement) {
	std::set< std::pair <int,int> > r;
	std::pair <int,int> tmp;
	
	// s'han de procesar els punts de img, i els del seu costat
	for(auto e : img) {
		for (int y = e.second-1; y <= e.second+1; y++) {
			for (int x = e.first-1; x <= e.first+1; x++) {
				tmp = std::make_pair(x, y);
				if (isBright(tmp, &img, enhancement)) r.insert(tmp);
			}
		}
	}
	
	return r;
}

void printLight(std::set< std::pair <int,int> > img) {
	int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
	for (auto e : img) {
		if (e.first < min_x) min_x = e.first;
		if (e.first > max_x) max_x = e.first;
		if (e.second < min_y) min_y = e.second;
		if (e.second > max_y) max_y = e.second;
	}
	
	for (int y  = min_y; y <= max_y; y++) {
		for (int x = min_x; x <= max_x; x++) write(1, (img.find( std::make_pair(x, y) ) != img.end()) ? "#" : ".", sizeof(char));
		write(1, "\n", sizeof(char));
	}
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char enhancement[ENHANCEMENT_LEN+2];
	std::set< std::pair <int,int> > img;
	
	/** /!\ és molt important que el primer caracter sigui '.' /!\
	 *  Si no, l'últim ha de ser '.' per tal que oscil·li
	 **/
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
			if (tmp == '#') img.insert( std::make_pair(x, y) );
			x++;
		}
		
		fscanf(file, "%c", &tmp);
	}
	
	printLight(img);
	img = enhance(img, enhancement);
	printf("\n");
	printLight(img);
	img = enhance(img, enhancement);
	printf("\n");
	printLight(img);

	printf("\n%ld\n", img.size());
	fclose(file);
}
