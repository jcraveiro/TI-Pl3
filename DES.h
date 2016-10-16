/*Author: Rui Pedro Paiva
Teoria da Informação, LEI, 2008/2009*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

unsigned char* encryptDES(unsigned char* inByteArray, long dim, uint64_t key, int type, long roundDim);
uint64_t encryptDESplain(uint64_t plain, uint64_t* subKeys);
void DESKeySchedule(uint64_t key, uint64_t* subKeys);
int DESgeneral (char* inFileName, unsigned long long key, int type);
unsigned char* signature(unsigned char* inByteArray, long dim, unsigned long long key);
int checkSignature(unsigned char* inByteArray, unsigned char* hash);

int DES (char* inFileName, unsigned long long key);
int unDES (char* inFileName, unsigned long long key);
void checkKeyStrength(uint64_t key);
void invertSubKeys(uint64_t* subKeys);
void checkKeyStrength(uint64_t key);
uint64_t permute(uint64_t value, int value_size, const unsigned int* table, int table_size);
uint64_t circularShift(uint32_t value, int value_size, int shift);
uint32_t feistelNetwork(uint32_t right, uint64_t subKey);
uint32_t subBoxes(uint64_t value);
