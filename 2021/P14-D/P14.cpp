#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <limits.h>
#include <map>

#define FILE_NAME "input.txt"

class Reaction {
private:
	unsigned short match;
	
	static unsigned short stoi(char char1, char char12) {
		return ((unsigned short)char1 << 8) | char12;
	}
	
	static unsigned short stoi(char *str) {
		return Reaction::stoi(str[0], str[1]);
	}
	
public:
	char add;
	
	Reaction(std::vector<char>::iterator match) {
		this->match = Reaction::stoi(*match, *(match+1));
	}
	
	Reaction(char *match, char add) {
		this->match = Reaction::stoi(match);
		this->add = add;
	}
	
	bool operator<(const Reaction &that) const {
		return this->match < that.match;
	}
};

void printVector(std::vector<char> list) {
	for (char &c : list) printf("%c", c);
	printf("\n");
}

std::pair<unsigned long long, unsigned long long> getLeastAndMostCommonAppearances(std::vector<char> result) {
	std::map<char,unsigned long long> apperances;
	std::map<char,unsigned long long>::iterator iter;
	for (char &c : result) {
		iter = apperances.find(c);
		if (iter == apperances.end()) apperances[c] = 1;
		else iter->second++;
	}
	
	std::pair<unsigned long long,unsigned long long> min_max = std::make_pair(0, 0);
	if (apperances.size() == 0) return min_max;
	
	iter = apperances.begin();
	min_max = std::make_pair(iter->second, iter->second);
	iter++;
	while (iter != apperances.end()) {
		if (min_max.first > iter->second) min_max.first = iter->second;
		if (min_max.second < iter->second) min_max.second = iter->second;
		
		iter++;
	}
	
	return min_max;
}

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	std::vector<char> result;
	std::vector<char>::iterator iter;
	std::set<Reaction> reactions;
	std::set<Reaction>::iterator search;
	char buffer[100], new_char, *tmp = buffer;
	
	fscanf(file, "%[^\n]%*[\n]", buffer);
	while (*tmp) result.push_back(*tmp++);
	
	fscanf(file, "%s -> %c", buffer, &new_char);
	while (!feof(file)) {
		reactions.insert(Reaction(buffer, new_char));
		
		fscanf(file, "%s -> %c", buffer, &new_char);
	}
	fclose(file);
	
	for (int x = 0; x < 40; x++) {
		iter = result.begin();
		while (iter < result.end()-1) {
			search = reactions.find(Reaction(iter));
			if (search != reactions.end()) {
				iter++; // skip added & position correctly
				iter = result.insert(iter, search->add);
			}
			iter++;
		}
		//printVector(result);
	}
	
	std::pair<unsigned long long, unsigned long long> solution = getLeastAndMostCommonAppearances(result);
	printf("%llu\n", solution.second - solution.first);
}
