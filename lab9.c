// Cristian McGee COP3502C Lab 9
#include <stdio.h>
#include <stdlib.h>
#define HASHSIZE 23

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// Fill out this structure
struct HashType {
  struct RecordType *data;
  struct HashType *next;
  int size;
};

// Compute the hash function
int hash(int x) { 
  return x % HASHSIZE; 
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
    // Implement parse data block
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
	printf("Displaying the records in the hash\n");
	for(int i = 0; i < hashSz; i++){
		printf("Index %d of the hashtable has %d elements\n", i, pHashArray[i].size);
		if(pHashArray[i].size != 0){
			printf("\t%d %c %d\n", pHashArray[i].data->id, pHashArray[i].data->name, pHashArray[i].data->order);
			struct HashType *temp = pHashArray[i].next;
			while(temp != NULL){
				printf("\t%d %c %d\n", temp->data->id, temp->data->name,temp->data->order);
				temp = temp->next;
			}
		}
	}
}

int main(void) {
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);
  
  struct HashType *pHashArray = malloc(HASHSIZE * sizeof(struct HashType));
  for(int i = 0; i < HASHSIZE; i++){
	pHashArray[i].data = NULL;
	pHashArray[i].next = NULL;
	pHashArray[i].size = 0;
  }
	for(int i = 0; i < recordSz; i++){
		int index = hash(pRecords[i].order);
		pHashArray[index].size ++;
		if(pHashArray[index].size == 1){
			pHashArray[index].data = &pRecords[i];
		} else if(pHashArray[index].size == 2){
			pHashArray[index].next = malloc(sizeof(struct HashType));
			pHashArray[index].next->data = &pRecords[i];
			pHashArray[index].next->next = NULL;
		} else {
			struct HashType *temp = pHashArray[index].next;
			struct HashType *prev = NULL;
			while(temp != NULL){
				prev = temp;
				temp = temp->next;
			}
			prev->next = malloc(sizeof(struct HashType));
			prev->next->data = &pRecords[i];
			prev->next->next = NULL;
		}
	}

	displayRecordsInHash(pHashArray, HASHSIZE);
  
}