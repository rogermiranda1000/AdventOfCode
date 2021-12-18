#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stack>

#define FILE_NAME "input.txt"
#define STR 150

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char line[STR], *ptr;
	unsigned int points = 0;
	bool invalid;
	
	fgets(line, STR, file);
	while(!feof(file)) {
		line[strlen(line)-1] = '\0';
		
		std::stack<char> wrappers;
		ptr = line;
		invalid = false;
		while(*ptr && !invalid) {
			switch(*ptr) {
				case '(':
					wrappers.push(')');
					break;
					
				case '[':
					wrappers.push(']');
					break;
					
				case '{':
					wrappers.push('}');
					break;
					
				case '<':
					wrappers.push('>');
					break;
				
				case ')':
				case ']':
				case '}':
				case '>':
					if (wrappers.empty()) {
						invalid = true;
						//std::cout << "S'ha trobat un '" << *ptr << "' extra" << std::endl;
					}
					else {
						if (wrappers.top() != *ptr) {
							invalid = true;
							//std::cout << "S'ha trobat un '" << *ptr << "', s'esperava un '" << wrappers.top() << "'" << std::endl;
						}
						wrappers.pop();
					}
					break;
			}
			
			if (invalid) {
				switch(*ptr) {
					case ')':
						points += 3;
						break;
						
					case ']':
						points += 57;
						break;
						
					case '}':
						points += 1197;
						break;
						
					case '>':
						points += 25137;
						break;
				}
			}
			
			ptr++;
		}
		
		/*if (!invalid) {
			// s'esperava més?
			if (!wrappers.empty()) {
				switch(wrappers.top()) {
					case ')':
						points += 3;
						break;
						
					case ']':
						points += 57;
						break;
						
					case '}':
						points += 1197;
						break;
						
					case '>':
						points += 25137;
						break;
				}
			}
		}*/
		
		fgets(line, STR, file);
	}
	
	printf("%u\n", points);
	fclose(file);
}
