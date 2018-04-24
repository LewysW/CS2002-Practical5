#ifndef HEADER
#define HEADER
#define READ 0
#define WRITE 1

typedef struct {
    FILE* file;
    char* name;
    long int size;
} FileStruct;

void secureSend(FileStruct message, FileStruct key, int fd[2]);
void secureReceive(FileStruct destination, int fd[2]);
int cipher(int m, int k);
void writeOutput(FILE* output, int cipherText);
FileStruct initKey(FileStruct key);
FileStruct initInput(FileStruct input, int keySize);
FileStruct initOutput(FileStruct output);
#endif
