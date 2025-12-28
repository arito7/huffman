build `g++ main.cpp -o huffman`

# improvements (todo)
1. use vector instead of map for frequency analysis

# steps
1. open and read input file
2. frequency analysis (generate a frequency map)
3. generate the huffman tree
4. generate the huffman codes
5. compress and write to output file
6. read compressed file and decompress into original string

# 1. Open and Read Input File
# 2. Frequency Analysis
# 3. Generate A Huffman Tree
# 4. Generate Huffman Codes
- recursively generate codes
- recursion allows us to remember the path we took to reach the leaf node
- we can also apply teh divide and conquer logic with recursion where we solve smaller problems to solve the big problem.
# 5. Compress and Write to Output File
- important to write the data as binary (or the entire thing is pointless)
- write the total character count at the beginning of the file so we know when to stop the decompression. otherwise the possible trailing 0's of the last byte can be decoded into unwanted characters.
- we also need to incldue the huffmancodes into the outputfile so we know how to decode it
- serialize the huffman tree and output
- the order the left and right child are processed is not important. it will only serve to change the 0 or 1 bit, and has no effect on the compression efficiency. the depth is the only thing that is taken into consideration in the context of compression. 
- when serializing and deserializing it is important to keep in mind which child you process first. while the order you process the left and right child does not initially make a difference, it does make a difference during the serialization/deserialization process as an identical tree needs to be reconstructed to decode. 
# 6. Decode
- deserialize huffman tree
- decode using the reconstructed huffman tree
# properties of a huffman tree

## mistakes and assumptions
- when serializing the huffman tree i assumed i needed the frequency but when decoding the frequency is not needed, only the shape of the tree.

## structural properties
- it is a full binary tree, meaning you will never see a node with just 1 child
- characters are only at the leaves, all internal nodes are structural placeholders containing the sum of frequencies
- N leaves = N - 1 internal nodes. Total nodes = 2N-1
## Algorithmic Properties (Logic)
    The Prefix Property: Because all characters are leaves, no character's code is a prefix of another character's code.

        Example: If 'a' is 0, then no other character can start with 0 (like 01 or 001). This is crucial because it allows us to decode the stream without separators. As soon as you trace a path to a leaf, you know the character is done.

    Optimality: A Huffman tree guarantees the minimum weighted path length.

    Frequent characters (high count) are closer to the root (short binary codes).

    Rare characters (low count) are deeper in the tree (long binary codes).

## Sibling Property: 
In a Huffman tree, the two nodes with the lowest frequencies are always siblings (they share the same parent). This is a direct result of the "combine smallest two" building process.
Tie-Breaking (The "Variation" Property)

While the efficiency of a Huffman tree is mathematically unique, the exact shape is not.

    If two characters have the same frequency (e.g., 'a': 2 and 'b': 2), you can swap them.

    The length of the codes will be the same, but the actual 0s and 1s might differ (e.g., 01 vs 10).

    This is why your output might look slightly different from mine, but the total number of bits in the compressed file should be identical.

## new keyword

if you don't use the new keyword when creating a struct it is made in stack memory, data created in stack memory dies when it reaches the closing curly bracket.
if you do use the new keyword it is allocated memory in the heap.
if you use the heap it will not be automatically garbage collected if you lose the pointer before deleting it you will create a memory leak.
