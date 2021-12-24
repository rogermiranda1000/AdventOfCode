#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FILE_NAME 	"input.txt"
#define INTEGER		"%[\\-0-9]"

typedef enum {
	COLLIDED,
	UNDER,
	OVER,
	UNREACHABLE
} Status;

typedef struct {
	int x1, y1, x2, y2;
} Area;

int getMaxCoordinate(int speed) {
	return speed*(speed+1)/2;
}

Status simulate(Area area, int x_speed, int y_speed) {
	bool x_zeroed = false;
	int x = 0, y = 0;
	while (true) {
		x += x_speed;
		y += y_speed;
		if (x_speed > 0) x_speed--;
		else x_zeroed = true;
		y_speed--;
		
		if (x <= area.x2 && y < area.y2) {
			if (x < area.x1) return (x_zeroed ? UNREACHABLE : UNDER);
			// està just a sota del area
			return (x_zeroed ? OVER : UNDER); // se li acaba d'acabar la velocitat en x, o ja estava caient?
		}
		if (x > area.x2) return OVER;
		if (x >= area.x1 && x <= area.x2 && y <= area.y1 && y >= area.y2) return COLLIDED;
		// els demés han de seguir iterant
	}
}

int getMaxY(Area area) {
	// donada una veloçitat inicial (y_speed0), l'objecte sempre acabarà a (x,0) amb veloçitat (x_speed,-y_speed0)
	// per tant, el màxim que pot arribar és -area.y2 (arribarà a (x,0) amb veloçitat area.y2, fent que coincideixi)
	return -area.y2;
}

int main() {
	Area area;
	char aux[10], aux2[10], aux3[10], aux4[10];
	
	int tmp;
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");
	fscanf(file, "target area: x=" INTEGER ".." INTEGER ", y=" INTEGER ".." INTEGER, aux, aux2, aux3, aux4);
	area.x1 = atoi(aux);
	area.x2 = atoi(aux2);
	if (area.x1 > area.x2) {
		tmp = area.x1;
		area.x1 = area.x2;
		area.x2 = tmp;
	}
	area.y1 = atoi(aux3);
	area.y2 = atoi(aux4);
	if (area.y1 < area.y2) {
		tmp = area.y1;
		area.y1 = area.y2;
		area.y2 = tmp;
	}
	fclose(file);
	
	/**
	 * max_x = x_speed*(x_speed+1)/2
	 *
	 * x_speed^2 + x_speed - 2*x1 >= 0
	 * x_speed^2 + x_speed - 2*x2 <= 0
	 *
	 * x_speed >= (-1 + sqrt(1 + 8*x1))/2
	 * x_speed <= (-1 + sqrt(1 + 8*x2))/2
	 **/
	int min_x_speed = ceil((-1.f + sqrt(1 + (8*area.x1)))/2.f), max_x_speed = (-1 + sqrt(1 + (8*area.x2)))/2;
	int max_y_speed = getMaxY(area);
	
	Status simulation;
	int best_y = 0;
	int y_speed;
	/*for (*/int x_speed = min_x_speed; /*x_speed <= max_x_speed; x_speed++) {*/
		y_speed = best_y+1;
		while ((simulation = simulate(area, x_speed, y_speed)) != UNREACHABLE) {
			if (simulation == COLLIDED) best_y = y_speed; // no cal mirar si y_speed > best_y, ja que partim d'aquesta
			y_speed++;
			if (y_speed > max_y_speed) break;
		}
	//}

	printf("%d\n", getMaxCoordinate(best_y));
}
