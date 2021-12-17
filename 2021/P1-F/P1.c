#include <stdlib.h>
#include <stdio.h>

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	int old, current;
	int count = 0;
	fscanf(file, "%d%*c", &old);
	while(!feof(file)) {
		fscanf(file, "%d%*c", &current);
		if (current > old) count++;
		old = current;
	}

	printf("%d\n", count);
	fclose(file);
}
