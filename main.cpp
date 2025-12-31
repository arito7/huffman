#include <cstdio>
#include <array>
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

static std::map<char, std::string> huffmanCodes;

void readAndPrintFile(std::ifstream &file){
	std::string line;
	
	std::cout << " Printing file contents " << std::endl;
	while(std::getline(file, line)){
		std::cout << line << std::endl;
	}
}

void generateCodes(Node * n, std::string code){
	if (n == nullptr){
		return;
	}
	if (n->left && n->right){
		generateCodes(n->left, code + '0');
		generateCodes(n->right, code + '1');
	} else {
		huffmanCodes[n->ch] = code;
		std::cout << n->ch << " : " << code << std::endl;
	} 
};

// pre order traversal DFS
void serializeHuffmanTree(Node * node, std::ofstream & outfile){
	if (!node) {
		return;
	}
	
	if (!node->right && !node->left){
		outfile.put('1');
		outfile.put(node->ch);
		return;
	} else {
		outfile.put('0');
		serializeHuffmanTree(node->left, outfile);
		serializeHuffmanTree(node->right, outfile);
	}
};

Node* deserializeHuffmanTree(std::ifstream &inputFile){
	char c;
	if(!inputFile.get(c)){
		std::cout << "deserialize .get failed" << std::endl;
		return nullptr;
	};
	if (c == '1'){
		char ch;
		inputFile.get(ch);
		return new Node(ch, 0);
	} else {
		Node * node = new Node('$', 0);
		node->left = deserializeHuffmanTree(inputFile);
		node->right = deserializeHuffmanTree(inputFile);
		return node;
	}
}

void huffmanSaveAndCompress(Node * huffmanTree, std::ifstream & iFile, std::ofstream & oFile){

	serializeHuffmanTree(huffmanTree, oFile);
	
	iFile.clear();
	iFile.seekg(0, std::ios::beg);
	unsigned char buffer = 0;
	int bitCount = 0;

	char c;
	while(iFile.get(c)){
		for (const char bit : huffmanCodes[c]){
			buffer = buffer << 1;
			bitCount += 1;
			if ( bit == '1' ) {
				buffer = buffer | 1;
			}

			if (bitCount == 8){
				oFile.put(buffer);
				buffer = 0;
				bitCount = 0;
			}
		}
	}
	if (bitCount > 0){
		while(bitCount < 8){
			buffer = buffer << 1;
			bitCount += 1;
		}
		oFile.put(buffer);
	}
};

void decodeHuffman(int totalCharacter, Node *root, std::ifstream &inputFile){

	Node * curr = root;

	std::string output = "";

	int count = 0;
	char byte;
	while (inputFile.get(byte)) {
		for (int i = 7; i >= 0; i--) {
			if (!curr->left || !curr->right){
				output = output + curr->ch;
				count++;
				curr = root;
				if(count >= totalCharacter){
					break;
				}
			}

			int bit = (byte >> i) & 1;

			if (bit == 0){
				if (curr->left){
					curr = curr->left;
				}
			} else {
				if (curr->right){
					curr = curr->right;
				}
			}
		}
	}
	std::cout << std::endl;
	std::cout << "Decoded output: ";
	std::cout << output << std::endl;
}

void printBTpreOrderDFS(Node * node){
	if (!node){
		return;
	}
	if(!node->right && !node->left){
		std::cout << node->ch;
	} else {
		printBTpreOrderDFS(node->left);
		printBTpreOrderDFS(node->right);
	}
}

void printHuffmanTree(Node * node){
	std::cout << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << std::endl;
	printBTpreOrderDFS(node);
	std::cout << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << std::endl;
}

void createFrequencyMap(std::array<int,256> & arr, std::ifstream &ifstream){
	char c;
	while(ifstream.get(c)){
		arr[c] = arr[c] + 1;
	}
}

Node * createHuffmanTree(std::array<int, 256> frequencyMap){
	std::priority_queue<Node *, std::vector<Node *>, Compare> minHeap;

	for (int i = 0; i < 256; i++){
		Node *node = new Node((char)i, frequencyMap[i]);
		minHeap.push(node);
	}

	while (minHeap.size() > 1){
		Node * right  = minHeap.top();
		minHeap.pop();
		Node * left = minHeap.top();
		minHeap.pop();
		Node * parent = new Node('$', left->freq + right->freq);
		parent->left = left;
		parent->right = right;
		minHeap.push(parent);
	}
	return minHeap.top();
}

int main(){
	std::cout << "Huffman project initialized" << std::endl;

	std::cout << "-----------------------------------------" << std::endl;
	std::cout << " Beginning Compression " << std::endl;
	std::cout << "-----------------------------------------" << std::endl;

	std::string inputFilePath = "input.txt";
	std::string outputFilePath = "compressed.bin";
	
	std::ifstream inputFile(inputFilePath);
	// because we aren't using the append flag, we won't be appending data to any previous output files
	std::ofstream outputFile(outputFilePath, std::ios::binary);

	if (!inputFile.is_open()){
		std::cerr << "Error: Could not open input file" << std::endl;
		return 1;
	}

	std::array<int, 256> frequency = {0};
	createFrequencyMap(frequency, inputFile);

	Node * huffmanTree = createHuffmanTree(frequency);

	printBTpreOrderDFS(huffmanTree);
	// write total char count to output file
	int totalCount = huffmanTree->freq;
	outputFile.write(reinterpret_cast<char*>(&totalCount), sizeof(int));

	generateCodes(huffmanTree, "");
	huffmanSaveAndCompress(huffmanTree, inputFile, outputFile);
	inputFile.close();
	outputFile.close();

	std::cout << "-----------------------------------------" << std::endl;
	std::cout << " Beginning Decompression " << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::ifstream iFile(outputFilePath, std::ios::binary);

	// read the total character count
	iFile.read(reinterpret_cast<char*>(&totalCount), sizeof(int));
	std::cout<<"Total Count: "<< totalCount <<std::endl;

	std::cout<<std::endl;

	std::cout << "\nDeserializing..." << std::endl;
	
	Node * root = deserializeHuffmanTree(iFile);

	printHuffmanTree(root);

	std::cout << "\nDecompressing..." << std::endl;

	decodeHuffman(totalCount, root, iFile);

	iFile.close();
	std::ifstream originalFile(inputFilePath);
	readAndPrintFile(originalFile);
	originalFile.close();

	return 0;
}

void printFrequencyTable(std::map<char, int> freqMap){
	std::cout << "Character Frequencies:" << std::endl;

	for (const auto& pair : freqMap){
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
}
