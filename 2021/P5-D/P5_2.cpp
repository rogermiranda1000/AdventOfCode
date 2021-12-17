#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <set>
#include <chrono>

#define FILE_NAME "input.txt"

using namespace std::chrono;

class Point {
private:
	unsigned int x,y;
	
	unsigned long long getUUID() const {
		return (((unsigned long long)this->x) << (4*8)) | this->y;
	}
	
public:
	Point(unsigned int x, unsigned int y) {
		this->x = x;
		this->y = y;
	}
	
	bool operator<(const Point& that) const {
		return this->getUUID() < that.getUUID();
	}
};

void drawMap(std::set<Point>& dots, int max_x, int max_y) {
	for (int y = 0; y <= max_y; y++) {
		for (int x = 0; x <= max_x; x++) {
			Point p = Point(x, y);
			std::cout << ((dots.find(p) != dots.end()) ? "x" : ".");
		}
		std::cout << "\n";
	}
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	std::set<Point> dots;
	std::set<Point> found;
	
	auto start = high_resolution_clock::now();
	int x1, y1, x2, y2, tmp;
	fscanf(file, "%d,%d -> %d,%d%*c", &x1, &y1, &x2, &y2);
	while(!feof(file)) {
		// x1,y1 han de ser menor a x2,y2
		if (x1 > x2) {
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		
		// afegir la linea
		if (x1 == x2 || y1 == y2) {
			if (y1 > y2) {
				tmp = y1;
				y1 = y2;
				y2 = tmp;
			}
			
			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					Point p = Point(x,y);
					if (!dots.insert(p).second) {
						// hi ha un repetit
						found.insert(p); // si està repetit i no s'ha sumat, incrementa
					}
				}
			}
		}
		else {
			// diagonal
			int increment = (y1 > y2) ? -1 : 1;
			for (int x = x1, y = y1; x <= x2; x++, y+=increment) {
				Point p = Point(x,y);
				if (!dots.insert(p).second) {
					// hi ha un repetit
					found.insert(p); // si està repetit i no s'ha sumat, incrementa
				}
			}
		}
		
		fscanf(file, "%d,%d -> %d,%d%*c", &x1, &y1, &x2, &y2);
	}
	
	//drawMap(dots, 9, 9);

	printf("%ld\n", found.size());
	
	auto stop = high_resolution_clock::now();
	std::cout << duration_cast<microseconds>(stop - start).count() << std::endl;
	
	fclose(file);
}
