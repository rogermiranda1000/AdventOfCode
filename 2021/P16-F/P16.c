#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define FILE_NAME 	"test.txt"//"input.txt"

#define ROUNDUP_DIV(up,down)	((up + (down-1)) / down)

uint8_t htoi(char hex) {
	if (hex >= '0' && hex <= '9') return hex - '0';
	return (hex - 'A') + 10;
}

void getInitialInfo(uint8_t *bit, uint8_t *version, uint8_t *type_id) {
	*version = (*bit >> 5);
	*type_id = (*bit >> 2) & (1<<3 - 1);
}

void discard(uint8_t **bit, size_t *bit_size, int discard_index) {
	*bit += discard_index/8;
	*bit_size -= 8*(discard_index/8);
	
	if (discard_index%8 == 0) return;
	(*bit)[0] <<= (discard_index%8);
	for (int x = 1; x < ROUNDUP_DIV(*bit_size, 8); x++) {
		(*bit)[x-1] |= (*bit)[x] >> (8 - discard_index%8);
		(*bit)[x] <<= (discard_index%8);
	}
	*bit_size -= discard_index%8;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	uint8_t *bits = NULL, current, *ptr, *end;
	size_t size = 0, bit_size;
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
	
	/*for (int x = 0; x < bit_size; x++) {
		for (int y = 128; y >= 1; y /= 2) printf("%c", (bits[x] & y) > 0 ? '1' : '0');
	}*/
	
	ptr = bits;
	end = bits+size;
	bit_size = size*8 - 4*half;
	uint8_t version, type_id, index;
	while (bit_size > 0) {
		getInitialInfo(ptr, &version, &type_id);
		index = 6;
		if (type_id == 4) {
			// paquet literal
			// 1+<trama 4 bits>+1+<trama 4 bits>+...+0+<trama 4 bits>
			while ((ptr[index/8] & 1<<(7 - (index%8))) > 0) {
				index++; // next
				index += 4; // data [no cal procesar-la encara]
			}
			
			// 0 (últim paquet)
			index++; // next
			index += 4; // data [no cal procesar-la encara]
			
			// trailing
			while (index % 4 != 0) index++;
		}
		else {
			
		}
		discard(&ptr, &bit_size, index);
		printf("%d %d\n", version, type_id);
	}
	
	fclose(file);
	free(bits);
}
