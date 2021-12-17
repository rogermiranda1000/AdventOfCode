#include <stdlib.h>
#include <stdio.h>

#define BITS 12
#define MASK (((unsigned int)1<<BITS) - 1)

int main() {
	FILE *file = NULL;
	file = fopen("input.txt", "r");

	char input[BITS+1];
	int ones_count[BITS] = {0}, total_count = 0;
	fscanf(file, "%s%*c", input);
	while(!feof(file)) {
		for (int x = 0; x < BITS; x++) ones_count[x] += (input[x] == '1');
		total_count++;
		
		fscanf(file, "%s%*c", input);
	}
	
	unsigned int gamma_rate = 0, epsilon_rate;
	for (int x = 0; x < BITS; x++) {
		gamma_rate *= 2;
		if (ones_count[x] > total_count/2) gamma_rate++;
	}
	epsilon_rate = (unsigned int)(~gamma_rate) & MASK;
	printf("%d %d\n", gamma_rate, epsilon_rate);

	printf("%d\n", gamma_rate*epsilon_rate);
	fclose(file);
}
