#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define FILE_NAME	"input.txt"
#define PRE_RESULT	10
#define RESULT		4

#define atoc(c) (1<<(c - 'a'))

uint8_t ato8_t(char *data) {
	uint8_t r = 0;
	
	while (*data) {
		r += atoc(*data);
		
		data++;
	}
	
	return r;
}

bool checkSegment(uint8_t sengmen_value, uint8_t mask) {
	return (sengmen_value & mask) == mask && (sengmen_value & ((uint8_t)~mask)) == 0;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");
	
	int index = 0;
	char buffer[8];
	uint8_t pre[PRE_RESULT], result[RESULT];
	
	fscanf(file, "%[^ \n]%*[ |\n]", buffer);
	while(!feof(file)) {
		if (index < PRE_RESULT) pre[index++] = ato8_t(buffer);
		else if (index < PRE_RESULT+RESULT) {
			result[index - PRE_RESULT] = ato8_t(buffer);
			index++;
		}
		else {
			// ja es te tota la linea
			for (int x = 0; x < 10; x++) printf("%d ", pre[x]);
			printf("| ");
			for (int x = 0; x < 4; x++) printf("%d ", result[x]);
			printf("\n");
			
			index = 0;
			continue; // salta el fscanf
		}
		
		fscanf(file, "%[^ \n]%*[ |\n]", buffer);
	}
	
	fclose(file);
}
