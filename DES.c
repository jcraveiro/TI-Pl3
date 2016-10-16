/*Author: Rui Pedro Paiva
  Teoria da Informação, LEI, 2008/2009*/

#include "DES.h"
#include "debug.c"

#define DEBUG 0

#define IP_SIZE                         64
#define PC1_SIZE                        28
#define PD1_SIZE                        28
#define PC2_SIZE                        48
#define IP_INVERSE_SIZE         64
#define E_SIZE                          48
#define P_SIZE                  32
#define S_SIZE                  64
#define WEAK_KEY_SIZE           2
#define SEMIWEAK_KEY_SIZE       12

const unsigned int IP  [IP_SIZE] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
const unsigned int PC1 [PC1_SIZE] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36};
const unsigned int PD1 [PD1_SIZE] = {63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
const unsigned int PC2 [PC2_SIZE] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
const unsigned int IP_INVERSE [IP_INVERSE_SIZE] = {40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};

const unsigned int E [E_SIZE] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
const unsigned int P [P_SIZE] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

const unsigned int R [16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

const unsigned int S[8][S_SIZE] = {
    {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
     0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
     4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
     15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},

    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
     3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
     0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
     13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},

    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
     13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
     13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
     1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},

    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
     13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
     10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
     3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},

    {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
     14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
     4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
     11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},

    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
     10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
     9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
     4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},

    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
     13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
     1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
     6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},

    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
     1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
     7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
     2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};

uint64_t weakKeys[WEAK_KEY_SIZE] = {0x0101010101010101,
                                    0xFEFEFEFEFEFEFEFE};

uint64_t semiWeakKeys[SEMIWEAK_KEY_SIZE] = {0x011F011F010E010E, 0x1F011F010E010E01,
                                            0x01E001E001F101F1, 0xE001E001F101F101,
                                            0x01FE01FE01FE01FE, 0xFE01FE01FE01FE01,
                                            0x1FE01FE00EF10EF1, 0xE01FE01FF10EF10E,
                                            0x1FFE1FFE0EFE0EFE, 0xFE1FFE1FFE0EFE0E,
                                            0xE0FEE0FEF1FEF1FE, 0xFEE0FEE0FEF1FEF1};

//função para encriptação
int DES (char* inFileName, unsigned long long key)
{
    return DESgeneral(inFileName, key, 0);
}


//função para decriptação
int unDES (char* inFileName, unsigned long long key)
{
    return DESgeneral(inFileName, key, 1);
}

long roundToMult(long value) {

    long final_value = value;

    while((final_value%8) != 0) {
        final_value++;
    }

    if (DEBUG == 1) {
        printf("Dimensions value needed to be increased by %ld bytes\n", (final_value-value));
    }
    
    return final_value;
}

//função geral para encriptação (type = 0) e decriptação (type = 1) de um ficheiro
int DESgeneral (char* inFileName, unsigned long long key, int type)
{
    FILE* DESInFile;
    unsigned char* inByteArray;
    long inFileSize;
    unsigned char* crpByteArray;
    char* outFileName;
    int write;
    char response;
    struct stat stFileInfo;
    FILE* DESOutFile;
    char suf[5];
    int i, trailer_counter;
    unsigned char* newCrpByteArray;
    long roundedInFileSize;

    //abrir ficheiro e ler tamanho
    DESInFile = fopen(inFileName, "rb");
    if (DESInFile == NULL)
    {
        printf("Error opening file for reading. Exiting...\n");
        return 1;
    }
    
    fseek(DESInFile, 0L, SEEK_END);
    inFileSize = ftell(DESInFile);  //ignore EOF
    fseek(DESInFile, 0L, SEEK_SET);
    roundedInFileSize = roundToMult(inFileSize);


    //ler ficheiro inteiro para array inByteArray
    inByteArray = (unsigned char*) calloc(roundedInFileSize, sizeof(unsigned char));
    fread(inByteArray, 1, inFileSize, DESInFile);

    //encriptar dados e assinatura no array
    if ((crpByteArray = encryptDES(inByteArray, inFileSize, key, type, roundedInFileSize)) == NULL) {
        return -1;
    }

    //flush do crpByteArray para ficheiro
    //nome do ficheiro de saída
    if (type == 0)  //encriptação
    {
        outFileName = (char*) calloc(strlen(inFileName) + 5, sizeof(char));
        strcpy(outFileName, inFileName);
        strcat(outFileName, ".DES");
    }
    else  //decriptação
    {
        strcpy(suf, &inFileName[strlen(inFileName) - 4]);
        if (strcmp(suf, ".DES") == 0)
        {
            outFileName = (char*) calloc(strlen(inFileName) + 5, sizeof(char));
            strcpy(outFileName, "DES_");
            strcat(outFileName, inFileName);
            outFileName[strlen(outFileName) - 4] = 0;
        }
        else
        {
            outFileName = (char*) calloc(14, sizeof(char));
            strcpy(outFileName, "DES_decrypted");
        }
    }


    if (type == 1) {
        //****************************************************************
        //ler o ultimo byte de crpByteArray e retirar esse numero de
        //bytes, no caso de serem todos iguais
        //****************************************************************
        trailer_counter = 0;
        
        if (DEBUG == 1) {
            printf("I am decripting... Found %d as the last byte\n", crpByteArray[inFileSize-1]);
            printf("Trying to remove %d bytes\n", crpByteArray[inFileSize-1]);
        }
        
        for (i = 0; i < crpByteArray[inFileSize-1]; i++) {
            if (crpByteArray[inFileSize-1-i] == crpByteArray[inFileSize-1]) {
                trailer_counter++;
            }
        }
        if (trailer_counter == crpByteArray[inFileSize-1]) {
            inFileSize -= crpByteArray[inFileSize-1];
            newCrpByteArray = (unsigned char*) calloc(inFileSize, sizeof(unsigned char));
            memcpy(newCrpByteArray, crpByteArray, inFileSize);
            free(crpByteArray);
            crpByteArray = newCrpByteArray;
            roundedInFileSize -= trailer_counter;
        }
    }

    //criar ficheiro
    write = 1;
    if(stat(outFileName, &stFileInfo) == 0) //see if file already exists
    {
        printf ("File already exists. Overwrite (y/n)?: ");
        response = getchar();
        if (response == 'n')
            write = 0;
        printf("\n");
        fflush(stdin);
    }

    if (write)
    {
        DESOutFile = fopen(outFileName, "wb");
        if (DESOutFile == NULL)
        {
            printf("Error opening file for writing!!! Exiting...\n");
            return -1;
        }
        fwrite(crpByteArray, 1, roundedInFileSize, DESOutFile);
        fclose(DESOutFile);
    }

    //finalizações
    free(inByteArray);
    free(outFileName);
    free(crpByteArray);
    fclose(DESInFile);

    return 0;
}


// função para encriptação/decriptação de dados no array inByteArray, de dimensão dim
unsigned char* encryptDES(unsigned char* inByteArray, long dim, uint64_t key, int type, long roundDim)
{
    uint64_t subKeys[16];
    unsigned char* outByteArray;
    uint64_t plain, cipher;
    int i, j, h;

    checkKeyStrength(key);

    //obtém sub-keys (16 de comprimento 48)
    DESKeySchedule(key, subKeys);

    if (DEBUG == 1) {
        print_array(subKeys, 16);
    }

    if (type == 1) //decrypt --> inverter subKeys
    {
        invertSubKeys(subKeys);
    }

    outByteArray = (unsigned char*) calloc(dim, sizeof(unsigned char));
    i = 0;
    plain = 0;
    while (i < dim)
    {
        plain = 0;
        j = i;
        while (j < i + 8 && j < dim)
        {
            plain = plain | ((uint64_t)inByteArray[j] << (64 - 8*(j-i+1)));
            j++;

        }

        //determina cifra
        if (j - i == 8)  //ficheiro é múltiplo de 8 bytes
            cipher = encryptDESplain(plain, subKeys);
        else {
            if (type == 0) {
                /*
                  Solucao viavel a 100% para plaintext ASCII e UTF-8;
                  Escolhemos este metodo por, apesar de poder dar "asneira" com ficheiros binarios,
                    ser o metodo que aplicado a qualquer tipo de ficheiro consideramos ter menor
                    probabilidade de erro;
                  Metodo escolhido:
                  ->ver quantos bytes faltam preencher;
                  ->escrever esse mesmo numero de bytes com esse numero, ate completar os 8;
                  -> e.x: supondo que faltam preencher 2 bytes, escreveriamos 0x02 0x02;
                */
                if (DEBUG == 1) {
                    printf("File size is not multiple... %d bytes to fill\n", 8-(j-i));
                }
                
                plain >>=  8 * (8-(j-i));
                for (h = 0; h < 8-(j-i); h++) {
                    plain <<= 8;
                    plain |= (uint64_t)(8-(j-i));
                }

                cipher = encryptDESplain(plain, subKeys);
            } else {
                printf("File to decrypt is not 8byte multiple in length\n");
                return NULL;
            }
        }

        //guarda cifra no array de saída
        j = i;
        while (j < i + 8 && j < roundDim)
        {
            outByteArray[j] = (unsigned char) (cipher >> (56 - 8*(j-i)) & (0xFF));
            j++;
        }

        i = j;
    }

    return outByteArray;
}


/************************************************************************************/
/***************************** ADICIONAR CóDIGO *************************************/
/************************************************************************************/

uint32_t subBoxes(uint64_t value) {

    int i;
    uint32_t final_value = 0x00000000;
    uint64_t column_mask = 0x000000000000001E;
    uint64_t line_mask   = 0x0000000000000001;
    uint8_t line;
    uint8_t column;
    uint64_t temp;

    for (i = 0; i < 8; i++) {

        //verificar qual a linha e coluna de interesse
        line = (uint8_t)((((line_mask << 5) & value) >> 4) | (line_mask & value));
        column = (uint8_t)((value & column_mask) >> 1);

        //seleccionar o valor de 4 bits da tabela
        temp = (uint32_t)(S[7-i][line*16 + column] & 0x0000000F);
        temp <<= i*4;

        final_value |= temp;

        value >>= 6;
    }
    
    return final_value;
}

uint32_t feistelNetwork(uint32_t right, uint64_t subKey) {

    uint64_t right_modified;
    uint32_t right_final;

    right_modified = permute(right, 32, E, E_SIZE);
    right_modified ^= subKey;

    right_final = subBoxes(right_modified);

    right_final = permute(right_final, 32, P, P_SIZE);

    return right_final;
}

//funcao que verifica se a chave é boa ou nao
void checkKeyStrength(uint64_t key) {
    int i, j;

    for (i = 0; i < WEAK_KEY_SIZE; i++) {
        if (key == weakKeys[i]) {
            printf("[Warning!] Your key is weak. Please consider changing it.\n");
            return;
        }
    }

    for (j = 0; j < SEMIWEAK_KEY_SIZE; j++) {
        if (key == semiWeakKeys[j]) {
            printf("[Warning!] Your key is semi-weak. It would be better to chose another one.\n");
            return;
        }
    }
}

//funcao para inverter as subKeys
void invertSubKeys(uint64_t* subKeys) {
    int i;
    uint64_t temp;

    for (i = 0; i < 8; i++) {
        temp = subKeys[15-i];
        subKeys[15-i] = subKeys[i];
        subKeys[i] = temp;
    }
}

//funcao que permuta um valor, de acordo com uma tabela
uint64_t permute(uint64_t value, int value_size, const unsigned int* table, int table_size) {
    int i, index;
    uint64_t mask = 0x0000000000000001;
    uint64_t temp_mask;
    uint64_t final_seq = 0x0000000000000000;

    mask = mask << (value_size-1);

    for (i = 0; i < table_size; i++) {
        index = table[i];
        temp_mask = mask >> (index-1);
        temp_mask &= value;
        temp_mask >>= (value_size-index);

        final_seq <<= 1;
        final_seq |= temp_mask;
    }

    return final_seq;
}

// função para encriptação de uma mensagem de 64 bits (plain), com base nas subKeys
//devolve a mensagem cifrada
uint64_t encryptDESplain(uint64_t plain, uint64_t* subKeys)
{
    uint32_t right, left, temp;
    int i;

    //permutacao inicial segundo IP
    plain = permute(plain, 64, IP, IP_SIZE);

    //divisao dos 64 bits em duas partes de 32 bits
    right = (uint32_t)(plain & 0x00000000FFFFFFFF);
    left = (uint32_t)((plain & 0xFFFFFFFF00000000) >> 32);

    //realizacao das 16 iteracoes do algoritmo
    for (i = 0; i < 16; i++) {
        temp = feistelNetwork(right, subKeys[i]);
        temp ^= left;

        left = right;
        right = temp;
    }

    /*
      concatenacao de right e left finais;
      a ordem sera right|left e nao left|right;
    */
    plain = 0x0000000000000000 | (uint64_t)right;
    plain = (plain << 32) | left;

    //permutacao final segundo a tabela inversa de IP
    plain = permute(plain, 64, IP_INVERSE, IP_INVERSE_SIZE);

    return plain;
}

uint64_t circularShift(uint32_t value, int value_size, int shift) {
    int i;
    uint32_t overflow;
    uint32_t mask = 0x00000001;
    uint32_t final_value = value;

    mask <<= (value_size-1);

    for (i =0 ; i < shift; i++) {
        overflow = mask & final_value;
        overflow >>= (value_size-1);

        final_value &= (mask ^ 0xffffffff);

        final_value <<= 1;
        final_value |= overflow;
    }

    return (uint64_t)final_value;
}

// função para gerar sub-keys (uma chave para cada uma das 16 iterações)
void DESKeySchedule(uint64_t key, uint64_t* subKeys)
{
    uint64_t c, d;
    uint64_t cd;
    int i;

    c = permute(key, 64, PC1, PC1_SIZE);
    d = permute(key, 64, PD1, PD1_SIZE);

    for (i = 0; i < 16; i++) {

        cd = 0x0000000000000000;

        //shift circular
        c = circularShift(c, 28, R[i]);
        d = circularShift(d, 28, R[i]);

        //concatenacao das duas partes
        cd = (c << 28) | d;
        cd = permute(cd, 56, PC2, PC2_SIZE);

        //armazenamento da subKey
        subKeys[i] = cd;
    }
}
