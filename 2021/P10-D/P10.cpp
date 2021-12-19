#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <queue>
#include <set>

#define FILE_NAME "input.txt"
#define STR 150

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	char line[STR], *ptr;
	std::set<unsigned long long> results;
	unsigned long long aux;
	bool invalid;
	
	fgets(line, STR, file);
	while(!feof(file)) {
		line[strlen(line)-1] = '\0';
		
		std::stack<char> wrappers;
		
		// corrupted?
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
			
			ptr++;
		}
		
		if (!invalid && !wrappers.empty()) {
			aux = 0;
			while(!wrappers.empty()) {
				aux *= 5;
				switch(wrappers.top()) {
					case ')':
						aux++;
						break;
						
					case ']':
						aux += 2;
						break;
						
					case '}':
						aux += 3;
						break;
						
					case '>':
						aux += 4;
						break;
				}
				wrappers.pop();
			}
			
			if(aux > 0 && !results.insert(aux).second) {
				std::cerr << "Repeated value" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		fgets(line, STR, file);
	}
	
	std::set<unsigned long long>::iterator it = results.begin();
	std::advance(it, (results.size()-1)/2);
	printf("%llu\n", *it);
	fclose(file);
}
