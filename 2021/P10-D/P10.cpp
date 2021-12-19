#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <queue>
#include <set>

#define FILE_NAME "test.txt"//"input.txt"
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
		
		wrappers.clear();
		if (!invalid) {
			aux = 0;
			ptr = &line[strlen(line)-1];
			while(ptr >= line) {
				switch(*ptr) {
					case ')':
						wrappers.push('(');
						break;
						
					case ']':
						wrappers.push('[');
						break;
						
					case '}':
						wrappers.push('{');
						break;
						
					case '>':
						wrappers.push('<');
						break;
					
					case '(':
					case '[':
					case '{':
					case '<':
						if (wrappers.empty() /*|| wrappers.top() != *ptr */) {
							// falta
							aux *= 5;
							std::cout << *ptr;
							switch(*ptr) {
								case '(':
									aux++;
									break;
									
								case '[':
									aux += 2;
									break;
									
								case '{':
									aux += 3;
									break;
									
								case '<':
									aux += 4;
									break;
							}
						}
						else wrappers.pop();
						break;
				}
				
				ptr--;
			}
			
			std::cout << " (" << aux << ")" << std::endl;
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
