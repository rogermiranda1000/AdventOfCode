#include <stdlib.h>
#include <stdio.h>

#define WINDOW 3 // mesures simultaneas en el pitjor dels casos

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	int old = -1;
	int buffer[WINDOW] = {0}, index = 1, tmp, acum_index = 0, acumulated = 1;
	int count = 0;
	fscanf(file, "%d%*c", &buffer[0]);
	while(!feof(file)) {
		fscanf(file, "%d%*c", &tmp);
		
		for (int x = 0; x <= index; x++) buffer[x] += tmp;
		if (index < WINDOW-1) index++;
		
		if (acumulated < WINDOW) acumulated++;
		else {
			if (old != -1) {
				if (buffer[acum_index] > old) count++;
			}
			old = buffer[acum_index];
			
			// reiniciem i pasem al seguent
			buffer[acum_index] = 0;
			acum_index = (acum_index+1) % WINDOW;
		}
	}

	printf("%d\n", count);
	fclose(file);
}
