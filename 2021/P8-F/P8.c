#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define FILE_NAME	"input.txt"
#define PRE_RESULT	10
#define RESULT		4
#define MAX_DIGITS	7
#define CANDIDATE_MASK ((1<<MAX_DIGITS)-1)

#define position(c)		(c - 'a')
#define atoc(c) 		(1<<position(c))

#define UNCHECKED 69
typedef struct {
	uint8_t value;
	uint8_t real_value;
} SegmentInfo;

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

/**
 * 2 -> 1
 * 3 -> 7
 * 4 -> 4
 * 5 -> 2,3,5
 * 6 -> 0,6,9
 * 7 -> 8
 **/
uint8_t countOnes(uint8_t value) {
	uint8_t r = 0;
	
	while (value > 0) {
		r++;
		value = value & (value-1);
	}
	
	return r;
}

/**
 * @retval true s'ha trobat el valor real
 * @retval false no s'ha trobat el valor real
 */
bool checkInput(SegmentInfo *data, uint8_t segment_mask[MAX_DIGITS]) {
	uint8_t ones = countOnes(data->value);
	uint8_t not_candidate = ~data->value & CANDIDATE_MASK;
	char *tmp;
	int tmp2;
	switch (ones) {
		case 2:
			data->real_value = 1;
			segment_mask[position('c')] &= data->value;
			segment_mask[position('f')] &= data->value;
			segment_mask[position('a')] &= not_candidate;
			segment_mask[position('b')] &= not_candidate;
			segment_mask[position('d')] &= not_candidate;
			segment_mask[position('e')] &= not_candidate;
			segment_mask[position('g')] &= not_candidate;
			return true;
			
		case 3:
			data->real_value = 7;
			segment_mask[position('a')] &= data->value;
			segment_mask[position('c')] &= data->value;
			segment_mask[position('f')] &= data->value;
			segment_mask[position('b')] &= not_candidate;
			segment_mask[position('d')] &= not_candidate;
			segment_mask[position('e')] &= not_candidate;
			segment_mask[position('g')] &= not_candidate;
			return true;
			
		case 4:
			data->real_value = 4;
			segment_mask[position('b')] &= data->value;
			segment_mask[position('c')] &= data->value;
			segment_mask[position('d')] &= data->value;
			segment_mask[position('f')] &= data->value;
			segment_mask[position('a')] &= not_candidate;
			segment_mask[position('e')] &= not_candidate;
			segment_mask[position('g')] &= not_candidate;
			return true;
			
		case 7:
			data->real_value = 8;
			// té tots enjegats -> no aporta res
			return true;
			
		case 6:
			// s'ha de trobar on està el segment que falta
			return false;
			tmp = "d0c6e9";
			while (*tmp) {
				if ((segment_mask[position(*tmp)] & data->value) == 0) {
					for (tmp2 = 0; tmp2 < MAX_DIGITS; tmp2++) {
						if (tmp2 == position(*tmp)) continue; // s'hauria de marcar com no-candidat, però com ja ha retornat 0 no importa
						segment_mask[tmp2] &= data->value;
					}
					data->real_value = tmp[1]-'0';
					return true;
				}
				tmp += 2;
			}
			return false;
			
		case 5:
			// s'ha de trobar on està el segment que falta
			return false;
			tmp = "bf2be3ce5";
			while (*tmp) {
				if ((segment_mask[position(*tmp)] & data->value) == 0 && (segment_mask[position(tmp[1])] & data->value) == 0) {
					for (tmp2 = 0; tmp2 < MAX_DIGITS; tmp2++) {
						if (tmp2 == position(*tmp) || tmp2 == position(tmp[1])) continue; // s'hauria de marcar com no-candidat, però com ja ha retornat 0 no importa
						segment_mask[tmp2] &= data->value;
					}
					data->real_value = tmp[2]-'0';
					return true;
				}
				tmp += 3;
			}
			return false;
			
		default:
			return false; // no hauria d'arribar mai aquí
	}
}

void printBinary(uint8_t enter) {
	int r = 0, index = 7;
	while (index >= 0) {
		write(1, (enter & (1<<index)) ? "1" : "0", sizeof(char));
		
		index--;
	}
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");
	
	int index = 0, x, y;
	bool done = true;
	char buffer[8];
	SegmentInfo data[PRE_RESULT+RESULT];
	uint8_t segment_mask[MAX_DIGITS]; // segment_mask[0] indica que al fer-li la AND amb el resultat i mirar si >0, aquest retornará si la 'a' eestà engegada; pot ser que no s'estigui segur
	
	int count = 0;
	
	fscanf(file, "%[^ \n]%*[ |\n]", buffer);
	while(!feof(file)) {
		if (index < PRE_RESULT+RESULT) data[index++] = (SegmentInfo){ato8_t(buffer), UNCHECKED};
		else {
			// ja es té tota la linea
			/*for (x = 0; x < PRE_RESULT; x++) printf("%d ", data[x].value);
			printf("| ");
			for (x = PRE_RESULT; x < PRE_RESULT+RESULT; x++) printf("%d ", data[x].value);
			printf("\n");*/
			
			for (x = 0; x < MAX_DIGITS; x++) segment_mask[x] = CANDIDATE_MASK; // tots poden ser qualsevol
			
			x = 0;
			//do {
			for (x = PRE_RESULT; x < PRE_RESULT+RESULT; x++) {
				//if (data[x].real_value == UNCHECKED) {
					if (!checkInput(&data[x], segment_mask)) done = false;
					/*else {
						printf("S'ha trobat %d (equivalent a %d)\n", data[x].real_value, data[x].value);
						for (y = 0; y < MAX_DIGITS; y++) {
							printf("Candidat a %c: ", 'a'+y);
							fflush(stdout);
							printBinary(segment_mask[y]);
							write(1, "\n", sizeof(char));
						}
					}*/
				//}
				/*x++;
				if (x >= PRE_RESULT+RESULT) x = 0;*/
			}
			//} while (!done);
			
			// contar el número de cops al resultat que surt 1, 4, 7 i 8
			for (x = PRE_RESULT; x < PRE_RESULT+RESULT; x++) {
				count += (data[x].real_value != UNCHECKED);
				//if (data[x].real_value == 1 || data[x].real_value == 4 || data[x].real_value == 7 || data[x].real_value == 8) count++;
			}
			
			index = 0;
			continue; // salta el fscanf
		}
		
		fscanf(file, "%[^ \n]%*[ |\n]", buffer);
	}
	
	// queda 1 pendent
	data[index++] = (SegmentInfo){ato8_t(buffer), UNCHECKED};
	
	for (x = 0; x < MAX_DIGITS; x++) segment_mask[x] = CANDIDATE_MASK; // tots poden ser qualsevol
			
	x = 0;
	//do {
	for (x = PRE_RESULT; x < PRE_RESULT+RESULT; x++) {
		if (data[x].real_value == UNCHECKED) {
			if (!checkInput(&data[x], segment_mask)) done = false;
			/*else {
				printf("S'ha trobat %d (equivalent a %d)\n", data[x].real_value, data[x].value);
				for (y = 0; y < MAX_DIGITS; y++) {
					printf("Candidat a %c: ", 'a'+y);
					fflush(stdout);
					printBinary(segment_mask[y]);
					write(1, "\n", sizeof(char));
				}
			}*/
		}
		/*x++;
		if (x >= PRE_RESULT+RESULT) x = 0;*/
	}
	//} while (!done);
	
	// contar el número de cops al resultat que surt 1, 4, 7 i 8
	for (x = PRE_RESULT; x < PRE_RESULT+RESULT; x++) {
		count += (data[x].real_value != UNCHECKED);
		//if (data[x].real_value == 1 || data[x].real_value == 4 || data[x].real_value == 7 || data[x].real_value == 8) count++;
	}
	
	printf("%d\n", count);
	fclose(file);
}
