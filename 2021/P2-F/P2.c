#include <stdlib.h>
#include <stdio.h>

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	int x = 0, y = 0;
	char instruction;
	int num;
	fscanf(file, "%c%*s %d%*c", &instruction, &num);
	while(!feof(file)) {
		switch(instruction) {
			case 'f':
				x += num;
				break;
				
			case 'd':
				y += num;
				break;
				
			case 'u':
				y -= num;
				break;
		}
		
		fscanf(file, "%c%*s %d%*c", &instruction, &num);
	}

	printf("%d\n", x*y);
	fclose(file);
}
