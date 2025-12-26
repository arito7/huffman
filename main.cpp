#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <queue>
#include <vector>

struct Node {
	char ch;
	int freq;
	Node * left;
	Node * right;

	Node(char c, int f) {
		ch = c;
		freq = f;
		left = nullptr;
		right = nullptr;
	}
};

struct Compare {
	bool operator()(Node * a, Node * b){
		return a->freq > b->freq;
	}
};

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

	std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;
	// std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;

	for (const auto& c : frequency){
		Node *node = new Node( c.first, c.second );
		minHeap.push(node);
	}


	std::cout << "printing minheap of size: " << minHeap.size() << std::endl;

	while (minHeap.size() > 1){
		Node * left  = minHeap.top();
		minHeap.pop();
		Node * right = minHeap.top();
		minHeap.pop();
		Node * parent = new Node('$', left->freq + right->freq);
		parent->left = left;
		parent->right = right;
		minHeap.push(parent);
		std::cout << left->ch << " : " << left->freq << std::endl;
		std::cout << right->ch << " : " << right->freq << std::endl;
		std::cout << parent->ch << " : " << parent->freq << std::endl;
	}

	return 0;
}
