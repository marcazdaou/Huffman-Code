// Marc Abi Zeid Daou.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>

#define MAX_TREE_HT 256 // Maximum height

// Structure for a node in the Huffman Tree
struct MinHeapNode {
    char data; 
    unsigned freq; 
    struct MinHeapNode *left, *right; 
};

// Structure for the Min Heap
struct MinHeap {
    unsigned size; // Current number of nodes 
    unsigned capacity; // Maximum number of nodes 
    struct MinHeapNode** array; 
};

// Function to create a new node
struct MinHeapNode* createNode(char data, unsigned freq) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    node->data = data; // character data
    node->freq = freq; //  frequency
    node->left = node->right = NULL; //  to NULL
    return node;
}

// Function to create a new Min Heap
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0; // Start with size 0
    minHeap->capacity = capacity; // Set the capacity
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*)); // Allocate memory for nodes
    return minHeap;
}

// Swap two nodes in the Min Heap
void swapNodes(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a; // Temp storage for node
    *a = *b; // Swap nodes
    *b = temp;
}

// Function to maintain the Min Heap property
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx; // Start current node
    int left = 2 * idx + 1; // Left child index
    int right = 2 * idx + 2; // Right child index

    // Check if left child exists and is smaller than the current node
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left; // Update smallest if left child is smaller
    }

    // Check if right child exists and is smaller than the current smallest
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right; // Update smallest if right child is smaller
    }

    // If the smallest is not the current node, swap and continue heapifying
    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest); // Recursively heapify affected subtree
    }
}

// Extract the node with the smallest frequency from the Min Heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0]; // Get smallest node
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; // Move last node to root
    minHeap->size--; // Reduce the size
    minHeapify(minHeap, 0);
    return temp; // Return the smallest node
}

// Insert a new node into the Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* node) {
    minHeap->size++; 
    int i = minHeap->size - 1; // Index of the new node

    // Traverse up to find the correct position for the new node
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; // Move parent down
        i = (i - 1) / 2; // Move up the tree
    }
    minHeap->array[i] = node; // Place the new node at its correct position
}

// Build a Min Heap from character frequencies
struct MinHeap* buildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size); // Create a Min Heap
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = createNode(data[i], freq[i]); // Create and add nodes
    }
    minHeap->size = size; // Set the size of the Min Heap	

    for (int i = (minHeap->size - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i); // Heapify each node
    }
    return minHeap; // Return the built Min Heap
}

// Build the Huffman tree from the given character frequencies
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top; 
    struct MinHeap* minHeap = buildMinHeap(data, freq, size); // Build initial Min Heap


    // Loop until there is only one node left in the Min Heap
    while (minHeap->size != 1) {
        // Extract the two nodes of lowest frequency
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        
        // Create a new internal node with the combined frequency
        top = createNode('$', left->freq + right->freq); // '$' indicates internal node
        top->left = left; // Link left child
        top->right = right; // Link right child
        insertMinHeap(minHeap, top); // Insert the new node back into the Min Heap
    }
    return extractMin(minHeap); // Return the final node, which is the root of the Huffman Tree
}

// Store Huffmn codes in a 2D array
 void storeCodes(struct MinHeapNode* root, int arr[], int top, char huffmanCodes[MAX_TREE_HT][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, huffmanCodes);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, huffmanCodes);
    }

    // leaf, store the code
   if (!root->left && !root->right) {
        for (int i = 0; i < top; i++) {
            huffmanCodes[(unsigned char)root->data][i] = arr[i] + '0';
        }
        huffmanCodes[(unsigned char)root->data][top] = '\0';
    }
}

 void printCharacter(char ch) {
    if (ch == '\n') {
        printf("LF");  // Line Feed
    } else if (ch == '\t') {
        printf("TAB"); // Tab character
    } else if (ch == ' ') {
        printf("SPACE"); // Space character
    } else if (ch < 32 || ch > 126) {
        // non-printable ASCII characters in hex 
        printf("\\x%02X", (unsigned char)ch);
    } else {
        // print them normally
        printf("%c", ch);
    }
}


// Print character frequencies and Huffman codes
void printFrequenciesAndCodes(char data[], int freq[], char huffmanCodes[MAX_TREE_HT][MAX_TREE_HT], int size) {
    
	for (int i = 0; i < size; i++) {
        // print
	printCharacter(data[i]);
	printf("\t\t%d\t\t%s\n", freq[i], huffmanCodes[(unsigned char)data[i]]);

    }
}


// Count the frequency 
void countCharacterFrequencies(const char *filename, int freq[], int debug) {
    FILE *file = fopen(filename, "r"); // Open the file
    if (!file) { // Check for file opening error
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) { // Read each character until EOF
        freq[ch]++; // Increment the freq 
    }
    fclose(file); // Close the file


    if (debug) {
        printf("Character Frequencies:\n");
        for (int i = 0; i < MAX_TREE_HT; i++) {
            if (freq[i] > 0) {
                printf("'%c' (%d): %d\n", i, i, freq[i]); // Print frequency of each character
            }
        }
    }
}

//  encode the input file using Huffman codes
void encodeFile(const char *inFile, const char *outFile, struct MinHeapNode *root, int debug) {
    char huffmanCodes[MAX_TREE_HT][MAX_TREE_HT]; // Array for Huffman codes
    int arr[MAX_TREE_HT], top = 0; // Array to store current code and its length

    // Generate Huffman codes
    storeCodes(root, arr, top, huffmanCodes);

    // Open input file for reading
    FILE *inputFile = fopen(inFile, "r");
    if (!inputFile) {
        fprintf(stderr, "Error opening input file: %s\n", inFile);
        exit(EXIT_FAILURE);
    }

    // Open output file for writing
    FILE *outputFile = fopen(outFile, "wb");
    if (!outputFile) {
        fprintf(stderr, "Error opening output file: %s\n", outFile);
        exit(EXIT_FAILURE);
    }

    unsigned char buffer = 0; // Buffer to hold bits
    int bufferIndex = 0; // Number of bits in buffer
    int printed[MAX_TREE_HT] = {0}; // Array to track printed characters

    // Read input file and encode 
    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        //  debug
        if (debug && printed[ch] == 0) {
            printf("Encoding character: '%c' (%d)\n", ch, ch); 
            printed[ch] = 1; // Mark this character as printed
        }

        // Find the Huffman code for the character
        for (int i = 0; i < MAX_TREE_HT; i++) {
            if (huffmanCodes[i][0] != 0 && i == (unsigned char)ch) { // Check for valid Huffman code
                for (int j = 0; huffmanCodes[i][j] != '\0'; j++) {
                    buffer = (buffer << 1) | (huffmanCodes[i][j] - '0'); // Add bit to buffer
                    bufferIndex++; // Increment bits in buffer

                    // If buffer 8 bits, write to the outfile
                    if (bufferIndex == 8) {
                        fwrite(&buffer, sizeof(buffer), 1, outputFile); // Write byte to file
                        buffer = 0; // Reset buffer
                        bufferIndex = 0; // Reset bit count
                    }
                }
                break; // Move to the next character
            }
        }
    }

    // Write bits in the buffer for outFile
    if (bufferIndex > 0) {
        buffer <<= (8 - bufferIndex); // Shift to fill remaining bits
        fwrite(&buffer, sizeof(buffer), 1, outputFile); // Write remaining bits
    }

    fclose(inputFile); // Close input file
    fclose(outputFile); // Close output file
}

// Main function

int main(int argc, char *argv[]) {

	char *inputFile = NULL;

	char *outputFile = NULL;
    	int c;
    	int debug = 0; // Debug variable

    // Set default filenames
    	const char *defaultInFile = "completeShakespeare.txt";
    	const char *defaultOutFile = "huffman.out";

// Process command-line options
    	while ((c = getopt(argc, argv, "i:o:d")) != -1) {
        	switch (c) {
            	case 'd':
                	debug = 1;
                	break;
            	case 'i':
                	inputFile = (char*)malloc(strlen(optarg) + 1);
                	strcpy(inputFile, optarg);
                	break;
            	case 'o':
                	outputFile = (char*)malloc(strlen(optarg) + 1);
               		strcpy(outputFile, optarg);
                	break;
            	default:
                	fprintf(stderr, "Usage: %s [-d] [-i input_file] [-o output_file]\n", argv[0]);
                	exit(EXIT_FAILURE);
        }
    }

// If no input file is provided, use the default
    	if (inputFile == NULL) {
        	inputFile = (char*)malloc(strlen(defaultInFile) + 1);
        	strcpy(inputFile, defaultInFile);
    }

    // If no output file is provided, use the default
    	if (outputFile == NULL) {
        	outputFile = (char*)malloc(strlen(defaultOutFile) + 1);
        	strcpy(outputFile, defaultOutFile);
    }


    	if (debug) {
        	printf("Debug mode is ON\n");
    }
    	printf("Input file: %s\n", inputFile);
    	printf("Output file: %s\n", outputFile);




    	int freq[MAX_TREE_HT] = {0};
    

    // Count character frequencies from the input file
    	countCharacterFrequencies(inputFile, freq, debug);

   
    	char data[MAX_TREE_HT]; // Array for characters
    	int frequency[MAX_TREE_HT]; // Array for frequencies
    	int size = 0; // Number of unique characters

    	for (int i = 0; i < MAX_TREE_HT; i++) {
        	if (freq[i] > 0) { // If character has frequency > 0
            	data[size] = (char)i; // Add character to data array
            	frequency[size] = freq[i]; // Add frequency to frequency array
            	size++; // Increment unique character count
        }
    }

    // Build the Huffman Tree
    	struct MinHeapNode* root = buildHuffmanTree(data, frequency, size);

    	char huffmanCodes[MAX_TREE_HT][MAX_TREE_HT]; 
    	int arr[MAX_TREE_HT], top = 0;


    // Generate Huffman codes
    	storeCodes(root, arr, top, huffmanCodes);


    // Print character frequencies and Huffman codes
    	printFrequenciesAndCodes(data, frequency, huffmanCodes, size);

    // Encode the file using Huffman codes
    	encodeFile(inputFile, outputFile, root, debug);

    	return 0; // Return success
}
