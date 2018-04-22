#ifndef HEADER
#define HEADER

typedef struct {
    FILE* file;
    char* name;
    long int size;
} FileStruct;

void secureSend(FileStruct message, FILE* key, FILE* destination);
int cipher(int m, int k);
void writeOutput(FILE* output, int cipherText);
FileStruct initKey(FileStruct key);
FileStruct initInput(FileStruct input, int keySize);
FileStruct initOutput(FileStruct output);
#endif
