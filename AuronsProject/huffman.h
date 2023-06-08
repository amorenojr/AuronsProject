#ifndef HUFFMAN_H
#define HUFFMAN_H

/* Including stdio so we'll know about FILE type */
#include <stdio.h>

#define MAX 256
#define BYTE_CHUNK_SIZE 1024
#define BYTE 8

// Note, I had to add this because the VS compiler is in C++ mode 
// and the linker needs to perform the proper steps to connect the
// method defs in this header, to the source file when it compiles.
#ifdef __cplusplus
extern "C" {
#endif

/* The following 2 functions are used in my huffencode and huffdecode
   programs. You'll need to write them if you want to use my code.  */

/**************************************************************/
/* Huffman encode a file.                                     */
/*     Also writes freq/code table to standard output         */
/* in -- File to encode.                                      */
/*       May be binary, so don't assume printable characters. */
/* out -- File where encoded data will be written.            */
/**************************************************************/
void encodeFile(FILE* in, FILE* out);

/*Symbols and Frequencies portion*/
void countSymbols(FILE *in, unsigned long *freq, unsigned long *charTotal);

/*not for final use*/
void printSymbols(unsigned long *freq, unsigned long charTotal);

/*Leaf and Priority queue creation*/
struct Tree
{
  unsigned long freq;
  unsigned char symbol;
  struct Tree* left;
  struct Tree* right;
  struct Tree* parentNode;
};

struct SortedList
{
  struct Tree* ptrToTree;
  struct SortedList* next;
  struct SortedList* previous;
};

/*create a node*/
struct Tree* createTreeNode(unsigned long freq, unsigned char symbol);
struct SortedList* createListNode(struct Tree* ptrToTree);

/*Takes a head node and a new node, inserts it into the correct position*/
struct SortedList* insertSortedEqualHelper(struct SortedList* current, struct SortedList* newNode);
struct SortedList* insertSorted(struct SortedList* head, struct SortedList* newNode);

/*Runs insertSorted in order to make the sorted list, takes in a head and a freq array, outputs head*/
struct SortedList* makeList(struct SortedList* head, unsigned long* freq);

/*prints out all of the nodes in sorted order, purely for debugging*/
void printList(struct SortedList* head);

/*makes huffman tree by combining nodes*/
struct SortedList* nodeComboHelper(struct SortedList* current);
struct Tree* buildHuffmanTree(struct SortedList* head);

void printHuffman(struct Tree* huffmanTree);

/*Creating codes from the huffman tree*/
/*Struct to store the codes*/ 
struct Code
{
  /*Need to know which leaf the code corresponds to*/
  struct Tree* ptrToTree;
  /*symbol here is a long instead of a character like in the Tree*/
  unsigned long symbol;
  unsigned long length;

  /*each Codes struct holds a singular code in long form and binary*/
  unsigned long code;

  unsigned char codeString[MAX-1];

};

/*Takes in the master nodeof the huffmanTree, also takes in a codes struct that has been given memory, this holds the length of the codes and the codes themselves*/
void makeCodes(struct Tree* huffmanTree, struct Code* codesList, unsigned long tempLength, unsigned long tempCode);

/*Function to print the Frequency and Code Table*/
void printTable(unsigned long* freq, unsigned long charTotal, struct Code* codesArray);

/*Encode Data using huffman code, write to output*/
/*looks at a character, pulls the corresponding code, appends that code to the binary output file*/
void writeEncodedText(FILE* in, FILE* out, struct Code* codesList);


/*Memory may need to be freed during all this but I have no Idea, maybe after its encoded? or decoded?*/


/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out);



#endif

#ifdef __cplusplus
}
#endif