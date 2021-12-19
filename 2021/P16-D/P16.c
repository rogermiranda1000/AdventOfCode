#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define FILE_NAME 	"input.txt"

#define ROUNDUP_DIV(up,down)	((up + (down-1)) / down)
#define BIT_ON(ptr,index)		((ptr[(index)/8] & 1<<(7 - ((index)%8))) > 0)

typedef struct {
	size_t lenght_sum;
	unsigned long long result;
} Header;

uint8_t htoi(char hex) {
	if (hex >= '0' && hex <= '9') return hex - '0';
	return (hex - 'A') + 10;
}

int get(uint8_t *bit, int bit_offset, int bits) {
	int r = 0;
	
	for (int x = 0; x < bits; x++) {
		r *= 2;
		r += (bit[(x+bit_offset)/8] & 1<<(7 - ((x+bit_offset)%8))) > 0;
	}
	
	return r;
}

void getInitialInfo(uint8_t *bit, int offset, uint8_t *version, uint8_t *type_id) {
	*version = get(bit, offset, 3);
	*type_id = get(bit, offset+3, 3);
}

unsigned long long getValue(unsigned long long *values, size_t values_size, uint8_t type) {
	unsigned long long aux;
	switch(type) {
		case 0:
			aux = 0;
			for (int x = 0; x < values_size; x++) {
				aux += values[x];
			}
			//printf("[v] Sum: %d\n", aux);
			return aux;
			
		case 1:
			aux = 1;
			for (int x = 0; x < values_size; x++) {
				aux *= values[x];
			}
			return aux;
			
		case 2:
			aux = values[0];
			for (int x = 1; x < values_size; x++) {
				if (values[x] < aux) aux = values[x];
			}
			return aux;
			
		case 3:
			aux = values[0];
			for (int x = 1; x < values_size; x++) {
				if (values[x] > aux) aux = values[x];
			}
			return aux;
			
		case 5:
			return values[0] > values[1];
			
		case 6:
			return values[0] < values[1];
			
		case 7:
			return values[0] == values[1];
	}
}

Header checkPackage(uint8_t *bits, int offset) {
	uint8_t version, type_id;
	getInitialInfo(bits, offset, &version, &type_id);
	Header r = {6 /* getInitialInfo() llegeix els 6 primers */, 0}, aux;
	
	if (type_id == 4) {
		// paquet literal
		// 1+<trama 4 bits>+1+<trama 4 bits>+...+0+<trama 4 bits>
		while (BIT_ON(bits, r.lenght_sum+offset)) {
			r.lenght_sum++; // next
			
			r.result <<= 4;
			r.result |= get(bits, r.lenght_sum+offset, 4);
			
			r.lenght_sum += 4;
		}
		
		// 0 (últim paquet)
		r.lenght_sum++; // no next
		
		r.result <<= 4;
		r.result |= get(bits, r.lenght_sum+offset, 4);
		//printf("[v] Literal: %d\n", r.result);
		
		r.lenght_sum += 4;
	}
	else {
		// operador
		bool subpackets = BIT_ON(bits, r.lenght_sum+offset);
		r.lenght_sum++;
		
		unsigned long long *results = NULL;
		size_t results_size = 0;
		if (subpackets) {
			// 11 bits indiquen el número de sub-paquets
			int pck = get(bits, r.lenght_sum+offset, 11);
			r.lenght_sum += 11;
			for (int x = 0; x < pck; x++) {
				aux = checkPackage(bits, r.lenght_sum+offset);
				r.lenght_sum += aux.lenght_sum;
				results = (unsigned long long*)realloc(results, sizeof(unsigned long long)*(++results_size));
				results[results_size-1] = aux.result;
			}
		}
		else {
			// 15 bits indiquen la longitud total
			int len = get(bits, r.lenght_sum+offset, 15), end;
			r.lenght_sum += 15;
			end = r.lenght_sum+len;
			while (r.lenght_sum < end) {
				aux = checkPackage(bits, r.lenght_sum+offset);
				r.lenght_sum += aux.lenght_sum;
				results = (unsigned long long*)realloc(results, sizeof(unsigned long long)*(++results_size));
				results[results_size-1] = aux.result;
			}
		}
		
		r.result = getValue(results, results_size, type_id);
		free(results);
	}
	
	return r;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	uint8_t *bits = NULL, current;
	size_t size = 0;
	char aux;
	bool half = false;
	
	fscanf(file, "%c", &aux);
	while (!feof(file) && aux != '\n') {
		current = htoi(aux);
		if (!half) {
			bits = (uint8_t*)realloc(bits, (++size));
			bits[size-1] = (current << 4);
		}
		else bits[size-1] |= current;
		half = !half;
		
		fscanf(file, "%c", &aux);
	}
	
	/*for (int x = 0; x < size; x++) {
		for (int y = 128; y >= 1; y /= 2) printf("%c", (bits[x] & y) > 0 ? '1' : '0');
	}
	printf("\n");*/
	
	printf("%llu\n", checkPackage(bits, 0).result);
	fclose(file);
	free(bits);
}
