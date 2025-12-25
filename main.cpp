#include <iostream>
#include <fstream>
#include <map>
#include <string>

int main(){
	std::cout << "Huffman project initialized" << std::endl;

	std::ifstream inputFile("input.txt");

	if (!inputFile.is_open()){
		std::cerr << "Error: Could not open input file" << std::endl;
		return 1;
	}

	std::map<char, int> frequency;

	char c;

	while(inputFile.get(c)){
		std::cout << c << std::endl;
		frequency[c]++;
	}

	std::cout << "Character Frequencies:" << std::endl;

	for (const auto& pair : frequency){
		char c = pair.first;
		int count = pair.second;

		std::cout << "'";
		
		if (c == '\n') {
			std::cout << "\\n";
		} else {
			std::cout << c;
		}

		std::cout << "' : " << pair.second << std::endl;
	}

	inputFile.close();

	return 0;
}
