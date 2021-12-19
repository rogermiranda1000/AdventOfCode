#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define FILE_NAME 	"input.txt"

#define ROUNDUP_DIV(up,down)	((up + (down-1)) / down)
#define BIT_ON(ptr,index)		((ptr[(index)/8] & 1<<(7 - ((index)%8))) > 0)

typedef struct {
	size_t lenght_sum;
	int version_sum;
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

Header checkPackage(uint8_t *bits, int offset) {
	uint8_t version, type_id;
	getInitialInfo(bits, offset, &version, &type_id);
	Header r = {6 /* getInitialInfo() llegeix els 6 primers */, version}, aux;
	
	printf("[v] v=%d t=%d\n", version, type_id);
	if (type_id == 4) {
		// paquet literal
		// 1+<trama 4 bits>+1+<trama 4 bits>+...+0+<trama 4 bits>
		while (BIT_ON(bits, r.lenght_sum+offset)) {
			r.lenght_sum++; // next
			//printf("*%d\n", get(bits, r.lenght_sum+offset, 4)); // data
			r.lenght_sum += 4;
		}
		
		// 0 (últim paquet)
		r.lenght_sum++; // no next
		//printf("\\%d\n", get(bits, r.lenght_sum+offset, 4)); // data
		r.lenght_sum += 4; // data [no cal procesar-la encara]
	}
	else {
		// operador
		bool subpackets = BIT_ON(bits, r.lenght_sum+offset);
		r.lenght_sum++;
		if (subpackets) {
			// 11 bits indiquen el número de sub-paquets
			int pck = get(bits, r.lenght_sum+offset, 11);
			r.lenght_sum += 11;
			for (int x = 0; x < pck; x++) {
				aux = checkPackage(bits, r.lenght_sum+offset);
				r.lenght_sum += aux.lenght_sum;
				r.version_sum += aux.version_sum;
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
				r.version_sum += aux.version_sum;
			}
		}
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
	
	printf("%d\n", checkPackage(bits, 0).version_sum);
	fclose(file);
	free(bits);
}
