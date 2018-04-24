#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "my_otp.h"

//TODO - make method for getting size of file.
//TODO - consider moving while loop and switch into method.

int main (int argc, char * argv []) {
    int opt;
    FileStruct input = {NULL, NULL, 0};
    FileStruct output = {NULL, NULL, 0};
    FileStruct key = {NULL, NULL, 0};

    if (argc == 1) {
        printf("Optional arguments appear in brackets '[]':\n");
        printf("Usage: my_otp [-i infile] [-o outfile] -k keyfile\n");
        return 0;
    }

    /*
    Gets arguments i, o, and k signifying the input file, output file and key file.
    */
    while ((opt = getopt(argc, argv, "i:o:k:")) != -1) {
        /*
        Gets optional arguments i and o for the input and output files, and gets
        the mandatory argument k specifying the key file.
        Prints a usage message if the key file is not specified.
        */
        switch (opt) {
            case 'i':
                if (optarg != NULL) input.name = strdup(optarg);
                break;
            case 'o':
                if (optarg != NULL) output.name = strdup(optarg);
                break;
            case 'k':
                if (optarg != NULL) key.name = strdup(optarg);
                break;
            case ':':
            case '?':
            default:
                printf("Optional arguments appear in brackets '[]':\n");
                printf("Usage: my_otp [-i infile] [-o outfile] -k keyfile.\n");
                exit(0);
                break;
        }
    }

    key = initKey(key);
    int fd[2];
    pipe(fd);

    pid_t child1 = fork();
    pid_t child2 = -1;
    if (child1 > 0) child2 = fork();

    if (child1 == 0) {
        input = initInput(input, key.size);
        secureSend(input, key, fd);
        fclose(input.file);
        remove("stdin.temp");
        _exit(0);
    }

    if (child2 == 0) {
        output = initOutput(output);
        secureReceive(output, fd);
        fclose(output.file);
        _exit(0);
    }

    int status;
    waitpid(child1, &status, 0);
    close(fd[WRITE]);
    waitpid(child2, &status, 0);
    close(fd[READ]);
    fclose(key.file);
    return 0;
}

/*
If key file is not specified then exit,
Else if key file cannot be accessed then exit,
Else open and read from key file, and get file size.
*/
FileStruct initKey(FileStruct key) {
    if (key.name == NULL) {
        printf("Optional arguments appear in brackets '[]':\n");
        printf("Usage: my_otp [-i infile] [-o outfile] -k keyfile.\n");
        exit(0);
    } else if (access(key.name, F_OK) == -1) {
        printf("Key file could not be read.\n");
        exit(0);
    } else {
        printf("Reading from key file...\n");
        key.file = fopen(key.name, "r");
        fseek(key.file, 0, SEEK_END);
        key.size = ftell(key.file);
        rewind(key.file);
    }

    return key;
}

/*
If no input file is specified or file cannot be accessed then revert to stdin,
else open and read from input file, and get file size.
*/
FileStruct initInput(FileStruct input, int keySize) {
    if (input.name == NULL || access(input.name, F_OK) == -1) {
        printf("Reverting to stdin for input...\n");
        FILE* temp = fopen("stdin.temp", "w");
        int c;
        while ((c = getchar()) != -1 && input.size < keySize) {
            fputc(c, temp);
            input.size++;
        }

        fclose(temp);
        input.file = fopen("stdin.temp", "r");
    } else {
        printf("Reading from input file...\n");
        input.file = fopen(input.name, "r");
    }

    fseek(input.file, 0, SEEK_END);
    input.size = ftell(input.file);
    rewind(input.file);

    return input;
}

/*
If no output file is specified then revert to stdout,
else open (or create) and write to output file.
*/
FileStruct initOutput(FileStruct output) {
    if (output.name == NULL) {
        printf("Reverting to stdout for output...\n");
        output.file = stdout;
    } else {
        printf("Writing to output file...\n");
        output.file = fopen(output.name, "w");
    }

    return output;
}

void secureSend(FileStruct message, FileStruct key, int fd[2]) {
    int c = 0; //Cipher char
    int count = 0;
    close(fd[READ]);

    while (count < message.size) {
        c = cipher(fgetc(message.file), fgetc(key.file));
        write(fd[WRITE], &c, sizeof(c));
        count++;
    }

    close(fd[WRITE]);

}

int cipher(int m, int k) {
    return (m^k);
}

void secureReceive(FileStruct destination, int fd[2]) {
    int c;
    close(fd[WRITE]);
    while(read(fd[READ], &c, sizeof(c))) writeOutput(destination.file, c);
    fclose(destination.file);
    close(fd[READ]);
}

void writeOutput(FILE* output, int cipher) {
    fputc(cipher, output);
    fflush(stdout);
}
