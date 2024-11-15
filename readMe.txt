Marc Abi Zeid Daou

This project wasn't easy. I spent a lot of time thinking about how to even begin. The first thing I did was set up getopt.h to correctly handle input and output file arguments. I struggled at first with ensuring that the default files, completeShakespeare.txt for input and huffman.out for output, were recognized properly. Debug mode really helped here, as it printed out the file names and confirmed that the defaults were working as expected.

Next came counting character frequencies, which I thought would be simple, but it required more thought than expected. I used the freq[] array to store the frequency of each character to handle every 256 possible ASCII characters. Debug mode was again essential, as it printed the frequency of each character after counting. I did ./huffman -d to see if my frequency matches.

I used the basic Huffman algorithm when building the Min Heap and the Huffman tree. I used MinHeap and MinHeapNode structures to create the priority queue, which allowed me to extract the characters with the lowest frequency first. The tree itself was built by combining nodes and inserting them back into the heap.

Generating Huffman codes from the tree was tricky. I used a 2D array huffmanCodes[][], where each row held the binary code for a specific character.I used void encodeFile(const char *inFile, const char *outFile, struct MinHeapNode *root, int debug) { to encode the file then I used a function called storecode to store them.

The hardest part, though, was encoding the input file using these Huffman codes. I knew I had the character frequencies and tree right, but when I ran the program using ./huff_Marc -i inputfile -o outputfile, it would only write 1024 bytes, despite the input file being much larger. I was baffled. After checking with ls tmp.out, the file had 331577 bytes, and yet my output stopped at 1024. It took me a while to figure out that the issue was with the buffer I used for writing bits to the output file.

I eventually realized that the buffer wasn't being flushed correctly. The buffer temporarily holds bits before writing them to the output file, and I hadn’t accounted for the remaining bits that didn’t fill up a full byte. By enabling debug mode, I saw that characters were correctly encoded, but only part of the file was being written. Once I fixed the buffer logic and ensured all bits were flushed, I could see that the encoded output was finally correct.
