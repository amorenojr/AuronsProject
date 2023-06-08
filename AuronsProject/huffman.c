/*************************************************/
/* Auron Joyner                                  */
/* April 28 2023                                 */
/* CS-241 Section #2DK                           */
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int i = 0;

/*Part 1*/

/*Count symbol frequencies and symbols*/
void countSymbols(FILE *in, unsigned long *freq, unsigned long *charTotal)
{
  
  /*Size thats being read in chunks and number of bytes read*/
  unsigned char byteChunk[BYTE_CHUNK_SIZE];
  unsigned long numberBytesRead;

  *charTotal = 0;
  /*setting memory*/
  memset(freq, 0, sizeof(unsigned long) * MAX);
  
  

  /*Adds to the frequency table for each symbol found, chunk at a time*/
  while((numberBytesRead = fread(byteChunk, 1, BYTE_CHUNK_SIZE, in)) >0)
  {
    /*first add to the character total*/
    *charTotal += numberBytesRead;
    /*Add to frequency*/      
    for(i = 0; i < numberBytesRead; i++)
    {
      freq[byteChunk[i]]++;
    }
  }
}

void printSymbols( unsigned long *freq, unsigned long charTotal)
{
  
  printf("Symbol\tFreq\tCode\n");
  for(i=0;i<MAX;i++)
    {
      if(freq[i]>0 && i<33)
	{
	  printf("=%d:\t%lu\n",i, freq[i]);
	}
      else if(freq[i]>0)
	{
	  printf("%c\t%lu\n",i,freq[i]);
	}
    }
  printf("Total chars = %lu\n", charTotal);
}




/********/
/*Part 2*/
/********/



/*to create a node*/
struct Tree* createTreeNode(unsigned long freq, unsigned char symbol)
{
  struct Tree* node = malloc(sizeof(struct Tree));

  node->freq = freq;
  node->symbol = symbol;
  node->left = NULL;
  node->right = NULL;
  node->parentNode = NULL;
  return node;
}

struct SortedList* createListNode(struct Tree* ptrToTree)
{
  struct SortedList* node = malloc(sizeof(struct SortedList));

  node->ptrToTree = ptrToTree;
  node->next = NULL;
  return node;
}

struct SortedList* insertSortedEqualHelper(struct SortedList* current, struct SortedList* newNode)
{
  
  struct Tree* currentTree = NULL;
  struct Tree* newTree = NULL;

  
  currentTree = current->ptrToTree;
  newTree = newNode->ptrToTree;
  
  /*Look for the leftmost symbol*/
  while(currentTree->left != NULL)
    {
      currentTree = currentTree->left;    
    }
  while(newTree->left != NULL)
    {
      newTree = newTree->left;
    }
  
  /*Once it gets to the leftmost node*/
  
  /*the head has a greater symbol so the newnode just goes in front*/
  if(currentTree->symbol > newTree->symbol)
    {
      /*reset to the top of that tree*/
      currentTree = current->ptrToTree;

      newNode->previous = current->previous;
      newNode->next = current;
      
      current = newNode;
      current->previous->next = current;
      
      return current;
    }
  /*the newnode goes after the current, so we need to keep sorting through*/
  if(currentTree->symbol < newTree->symbol)
    {
      if(current->next ==NULL)
	{
	  current->next = newNode;
	  return current;
	}
      
      /*take a look at the next node to see if its greater freq*/
      currentTree = current->next->ptrToTree;
      if(currentTree->freq > newNode->ptrToTree->freq)
	{
	  newNode->next = current->next;
	  current->next = newNode;
	  return current;
	}


      current->next->previous= current;
     
      current->next = insertSortedEqualHelper(current->next, newNode);
      return current;
    }
	  
  return current;
}



/*Sorts the list and returns the head of that list*/
struct SortedList* insertSorted(struct SortedList* head, struct SortedList* newNode)
{
  
  struct SortedList* current = head;
  


  if(current == NULL)
    {
      head = newNode;
      return head;
    }
  /*New has a greater frequency so it is put in next. Sort through all until it finds its position or its in last space*/
  while(current != NULL && current->ptrToTree->freq  < newNode->ptrToTree->freq)
    {
      
      if(current->next == NULL)
	{
	  current->next = newNode;
	  
	  return head;
	}
      current->next->previous = current;
      
      
      current = current->next;

      if(current->ptrToTree->freq > newNode->ptrToTree->freq)
	{
	  current->previous->next = newNode;
          newNode->next = current;

          return head;
	}
      
    }
  /*Current has a greater frequency so new goes in front of it*/
  if(current->ptrToTree->freq > newNode->ptrToTree->freq)
    {
      newNode->next = current;
      head = newNode;
      return head;
    }
  /*Helper function*/
  if(current->ptrToTree->freq == newNode->ptrToTree->freq)
    {
      current = insertSortedEqualHelper(current, newNode);
      
      /*Have to ensure that the previous of the original "current" becomes the previous of the new "current"*/
      current = head;
      return head;
    }
    
  else printf("didnt meet any if statements");
  return head;
}

struct SortedList* makeList(struct SortedList* head, unsigned long* freq)
{
  struct Tree* newTree;
  struct SortedList* newNode;
  
  for(i=0;i<MAX;i++)
    {
      if(freq[i] > 0)
	{
	  newTree = createTreeNode(freq[i],i);
          newNode =  createListNode(newTree);
	  
	  head = insertSorted(head,newNode);
	}
    }
  return head;
}


void printList(struct SortedList* head)
{
  while(head != NULL)
    {
      if(head->ptrToTree->symbol == 0)
	{
	  printf("Symbol: Blank, Freq: %lu\n",head->ptrToTree->freq);
	}
      else printf("Symbol: %lc, Freq: %lu\n",head->ptrToTree->symbol,head->ptrToTree->freq);
      head = head->next;
    }
}




/********/
/*Part 3*/
/********/

struct SortedList* nodeComboHelper(struct SortedList* current)
{
  struct Tree* combTree = NULL;
  struct Tree* currentTree = NULL;
  struct Tree* nextTree = NULL;
  struct SortedList* combNode = NULL;
  unsigned long combFreq=0;
  
  
  /*sets the first two nodes to be worked with and removes them from the sortedList*/
  currentTree = current->ptrToTree;
  nextTree = current->next->ptrToTree;
  current = current->next->next;
  
  combFreq = currentTree->freq + nextTree->freq;
  combTree = createTreeNode(combFreq,0);
  combTree->left = currentTree;
  combTree->right = nextTree;
  
  currentTree->parentNode = combTree;
  nextTree->parentNode = combTree;

  combNode = createListNode(combTree);

  /*Sort in the combo Node*/
  current = insertSorted(current, combNode);

  /*should return the head of a new priority queue*/
  return current;
}

struct Tree* buildHuffmanTree(struct SortedList* head)
{
  struct SortedList* current = head;
  struct Tree* combTree = NULL;
  struct Tree* currentTree = NULL;
  struct Tree* nextTree = NULL;
  struct SortedList* combNode = NULL;

  if(current == NULL)
  {
    printf("Error: Head is null when building huffman\n");
    exit(1);
  }
  
  while(current->next->next != NULL)
    {
      current = nodeComboHelper(current);
      
      printf("New List\n");
      printList(current);
    }
  /*must be just two left*/
  if(current->next!=NULL)
    {

      /*nodeComboHelper without setting the current two over*/
      /*sets the first two nodes to be worked with and removes them from the sortedList*/
      currentTree = current->ptrToTree;
      nextTree = current->next->ptrToTree;
      current = NULL;

      combTree = createTreeNode(currentTree->freq + nextTree->freq, 0);
      combTree->left = currentTree;
      combTree->right = nextTree;
      
      currentTree->parentNode = combTree;
      nextTree->parentNode = combTree;
      
      combNode = createListNode(combTree);
      current = combNode;   
      
      /*No longer a priority queue, should just have the master rootnode*/
      if(current->next!= NULL)
	{
	  printf("Failure: Tree has more than one node\n");
	}
    }
  if(current->next == NULL) printf("Success: only one node\n");
  currentTree= current->ptrToTree;
  /*must just be one left so the current should be the most combined node*/
  return currentTree;
}

void printHuffman(struct Tree* huffmanTree)
{
  struct Tree* root = huffmanTree;
  while(root!=NULL)
    {
      /*print root, then left then right, then left and right of left, then left and right of right*/
      printf("TreeSymbol: %lc, Freq: %lu\n",root->symbol,root->freq);
      if(root->left != NULL)
	{
	  printf("TreeSymbol: %lc, Freq: %lu\n",root->left->symbol,root->left->freq);
	}
      if(root->right != NULL)
	{
          printf("TreeSymbol: %lc, Freq: %lu\n",root->right->symbol,root->right->freq);
	}
      if(root->left!=NULL)
	{
	  printHuffman(root->left);
	}
      if(root->right!=NULL)
	{
	  printHuffman(root->right);
	}
    }
  printf("Tree cant print\n");
}
      




/********/
/*Part 4*/
/********/


/* A function that makes a single code that is attached to a tree node, meaning we need another way to make all codes*/
/*codesList should be filled*/
void makeCodes(struct Tree* huffmanTree, struct Code* codesList, unsigned long tempLength, unsigned long tempCode)
{
  int workingSymbol;
  unsigned long left;
  unsigned long right;
  
  /*Go through the nodes connected to the huffmanTree*/
  if(huffmanTree == NULL)
    {
      printf("No input to make codes\n");
      exit(1);
    }

  /*If it has reached the end of the branch it should have a symbol*/
  /*Set the symbol for the code, set the code, set the length of code*/
  if(huffmanTree->left == NULL && huffmanTree->right == NULL)
    {
      /*Lets us access the symbol and freq*/
      codesList->ptrToTree = huffmanTree;

      /*Need the symbol to be viewed as the decimal version*/
      workingSymbol = codesList->ptrToTree->symbol;

      /*It is an array of codes, so we need to set the value at the current position in the array*/
      /*the symbol at the position of workingSymbol in the codesList is workingSymbol*/
      codesList[workingSymbol].symbol = workingSymbol;
      codesList[workingSymbol].code = tempCode;

      /*Record the code length*/
      codesList[workingSymbol].length = tempLength;

      for(i=0; i < tempLength; i++)
	{
	  /*extracts the rightmost bit, if its not 0 it gives '1', if its 0 it gives '0'*/
	  codesList[workingSymbol].codeString[i] = ((tempCode >> (tempLength - 1 - i)) & 1) ? '1' : '0';
	}
      codesList[workingSymbol].codeString[tempLength] = '\0';
         
    }
  else
    {
      /*to traverse down the tree*/
      if(huffmanTree->left != NULL)
	{
	  left = tempCode << 1;
	  makeCodes(huffmanTree->left,codesList, tempLength + 1, left);
	}
      if(huffmanTree->right != NULL)
	{
	  right = tempCode << 1;
	  makeCodes(huffmanTree->right,codesList, tempLength + 1, right | 1);
	}
    }
}



void printTable(unsigned long* freq, unsigned long charTotal, struct Code* codesList)
{
  printf("Symbol\tFreq\tCode\n");
  for(i=0;i<MAX;i++)
    {
      if(freq[i]>0 && i<33)
	{
	  printf("=%d:\t%lu\t%s\n",i, freq[i], codesList[i].codeString);
	}
      else if(freq[i]>0)
	{
	  printf("%c\t%lu\t%s\n",i,freq[i], codesList[i].codeString);
	}
    }
  printf("Total chars = %lu\n", charTotal);
}



/********/
/*Part 5*/
/********/

void writeEncodedText(FILE* in, FILE* out, struct Code* codesList)
{
  unsigned char byteChunk[BYTE_CHUNK_SIZE];
  unsigned long numberBytesRead;
  
  unsigned long bitBuffer = 0;
  unsigned long bitCount = 0;
  unsigned char bit;
  
  unsigned char symbol;
  unsigned long i, j;
  

  struct Code code;
  /*Reads all of the file, looks at the character, changes it to the code*/
  /*SOMETHING PREVENTING THIS WHILE LOOP*/
  printf("Before while loop\n");
  fseek(in, 0, SEEK_SET);   // <--- This is the solution to the problem.
  while((numberBytesRead = fread(byteChunk, 1, BYTE_CHUNK_SIZE, in)) >0)
  {
    printf("start 1st while statement\n");
    for (i = 0; i < numberBytesRead; i++)
    {
      printf("First for loop\n");
      printf("%lu\n",i);
      /*ith place in the current chunk*/
      symbol = byteChunk[i];

      /*code Struct that needs to be appended*/
      code = codesList[symbol];

      /*Now, look through the code in the struct(an unsign long) and write it to the file as binary*/
      /*goes through the code one digit at a time*/
      for (j = 0; j < code.length; j++)
      {
	printf("%lu\n",code.code);
	/*sets bit equal to the jth bit in the code*/
        bit = (code.code >> (code.length - 1 - j)) & 1;

	/*adds the bit to the bitbuffer*/
        bitBuffer = (bitBuffer << 1) | bit;
        bitCount++;

        
        if (bitCount == BYTE)
        {
          fwrite(&bitBuffer, 1, 1, out);
          printf("%lu ", bitBuffer);
          bitBuffer = 0;
          bitCount = 0;
        }
      }
    }
  }
  printf("After while loop\n");

  
  if (bitCount > 0)
  {
    bitBuffer <<= (BYTE - bitCount);

    fwrite(&bitBuffer, 1, 1, out);
    printf("%lu ", bitBuffer); 
  }
}  

void encodeFile(FILE *in, FILE* out)
{
  unsigned long* freq;
  unsigned long charTotal;
  unsigned long tempLength = 0;
  unsigned long tempCode = 0;
  
  struct SortedList* head = NULL;
  struct Tree* huffmanTree =NULL;
  /*List to store all codes composed of Codes of the code struct*/
  struct Code* codesList = malloc(sizeof(struct Code) * MAX);

  /*initialize freq*/
  freq = malloc(MAX * sizeof(unsigned long));

  /*Initialize codesList, for all 255 symbols*/
  
  for(i=0;i<MAX;i++)
    {
      codesList[i].symbol = -1;
      codesList[i].code = 0;
      codesList[i].length = 0;
    }
  
  countSymbols(in,freq,&charTotal);
  printSymbols(freq,charTotal);
  
  printf("Initiate part 2\n\n");
  head = makeList(head,freq);
  printf("Made list\n");
  printList(head);
  
  printf("Initiate part 3\n\n");
  printf("Start Tree\n");
  huffmanTree = buildHuffmanTree(head);
  printf("Tree made\n");
  printf("Huffman Tree: Symbol: %lc , Freq: %lu \n",huffmanTree->symbol, huffmanTree->freq);

  printf("Initiate part 4\n\n");
  makeCodes(huffmanTree, codesList, tempLength,tempCode);
  printf("Made all codes\n");
  printTable(freq, charTotal, codesList);

  printf("Initiate part 5\n\n");
  /*charTotal is the total number of encoded symbols*/
  /*Code stream is the list of codes that corresponds to the initial uncoded data. put them into 8 character long bytes. convert those to int then to hex?*/
  writeEncodedText(in, out, codesList);
  printf("finish\n");
  
  
}
