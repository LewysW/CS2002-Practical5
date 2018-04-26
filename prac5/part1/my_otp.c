#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "my_otp.h"

int main (int argc, char * argv []) {
    int opt;

    /**
    Sets the initial values for the file, name, and size attributes of the
    input, output, and key structs.
    */
    FileStruct* input = initFileStruct();
    FileStruct* output = initFileStruct();
    FileStruct* key = initFileStruct();

    //If no options are specified then print usage message.
    if (argc == 1) showUsage();

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
                if (optarg != NULL) input->name = strdup(optarg);
                break;
            case 'o':
                if (optarg != NULL) output->name = strdup(optarg);
                break;
            case 'k':
                if (optarg != NULL) key->name = strdup(optarg);
                break;
            case ':':
            case '?':
            default:
                showUsage();
                break;
        }
    }

    /*Initialises the key struct used to encrypt data as well as the
    pipe to be written to. */
    key = initKey(key);
    int fd[2];
    pipe(fd);

    //Forks two child process responsible for executing encryption, reading and writing.
    pid_t child1 = fork();
    pid_t child2 = -1;
    if (child1 > 0) child2 = fork();

    //Carries out execution of encryption, reading and writing.
    executeChildProcesses(child1, child2, input, output, key, fd);

    //Waits for processes to finish and closes remaining open files.
    waitpid(child1, NULL, 0);
    close(fd[WRITE]);
    waitpid(child2, NULL, 0);
    close(fd[READ]);
    fclose(key->file);
    return 0;
}

/** initialises a file struct.
Allocates space for a file struct and sets the initial values of the file and name
to NULL and the size of the file to 0.
*/
FileStruct* initFileStruct() {
    FileStruct* fileStruct = malloc(sizeof(fileStruct));
    fileStruct->file = NULL;
    fileStruct->name = NULL;
    fileStruct->size = 0;
    return fileStruct;
}

//Prints usage message to user if options are invalid.
void showUsage() {
    printf("Optional arguments appear in brackets '[]':\n");
    printf("Usage: my_otp [-i infile] [-o outfile] -k keyfile.\n");
    exit(1);
}

/** Executes encryption, reading and writing carried out by the child prcoesses.
@child1 - id of first child process, @child2 - id of second child process,
@input - file struct of input file, @output - filestruct of output file,
@key - file struct of key file, @fd[] - file descriptors of pipe.
*/
void executeChildProcesses(pid_t child1, pid_t child2, FileStruct* input,
                            FileStruct* output, FileStruct* key, int fd[2]) {
    /*
    If current process is first child then initialise the input file struct and
    securely send the message using the input file, key and pipe. Also close the
    input file when finished and delete any temporary files. Finally exit.
    */
    if (child1 == 0) {
        input = initInput(input, key->size);
        secureSend(input, key, fd);
        fclose(input->file);
        remove("stdin.temp");
        _exit(0);
    }

    /*
    If current process is second child then initialise output file struct and
    securely receive the message using the output file struct and pipe. Then
    close output file and exit.
    */
    if (child2 == 0) {
        output = initOutput(output);
        secureReceive(output, fd);
        fclose(output->file);
        _exit(0);
    }
}

/* Initialises the key file struct.
Prints the usage if the file is not specified otherwise intialises file struct.
@key - key file struct to initialise.
*/
FileStruct* initKey(FileStruct* key) {
    //If file is NULL then usage is printed.
    if (key->name == NULL) {
        showUsage();
    //else if file cannot be read then exit
    } else if (access(key->name, F_OK) == -1) {
        printf("Key file could not be read.\n");
        exit(0);
    //Otherwise open key file, get size, and reset file.
    } else {
        printf("Reading from key file...\n");
        key->file = fopen(key->name, "r");
        fseek(key->file, 0, SEEK_END);
        key->size = ftell(key->file);
        rewind(key->file);
    }

    return key;
}

/* Initialises the input file struct.
Reverts to stdin if file is not specified, otherwise uses input file specified in options.
@input - input file to initialise, @keySize - size of key file.
*/
FileStruct* initInput(FileStruct* input, int keySize) {
    //If name is NULL or file cannot be accessed...
    if (input->name == NULL || access(input->name, F_OK) == -1) {
        //...then revert to stdin...
        printf("Reverting to stdin for input...\n");
        //...Create temporary file to write to...
        FILE* temp = fopen("stdin.temp", "w");
        int c;
        //Write input from stdin to temp file for program compatiblity
        while ((c = getchar()) != -1 && input->size < keySize) {
            fputc(c, temp);
            input->size++;
        }
        //Close temp file pointer and set input file to the new file.
        fclose(temp);
        input->file = fopen("stdin.temp", "r");
    //Otherwise open specified input file.
    } else {
        printf("Reading from input file...\n");
        input->file = fopen(input->name, "r");
    }

    //Get size of input
    fseek(input->file, 0, SEEK_END);
    input->size = ftell(input->file);
    rewind(input->file);

    return input;
}

/* Initialises the output file struct.
Reverts to stdout if no file is specified, otherwise uses output file specified in options.
@output - output file strict to initialise
*/
FileStruct* initOutput(FileStruct* output) {
    //If no output file is specified...
    if (output->name == NULL) {
        //..then revert to stdout for output
        printf("Reverting to stdout for output...\n");
        output->file = stdout;
    //Otherwise open specified output file.
    } else {
        printf("Writing to output file...\n");
        output->file = fopen(output->name, "w");
    }

    return output;
}

/* Encrypts data character by character and writes each character to the pipe.
@message - message to securely send over pipe, @key - key to use for one time pad
encryption. @fd - pipe file descriptors required for pipe reading and writing.
*/
void secureSend(FileStruct* message, FileStruct* key, int fd[2]) {
    int c = 0; //Cipher char
    int count = 0;
    //Closes read end as this process writes to pipe.
    close(fd[READ]);

    /*Encrypts and writes characters to pipe while number of characters written
    is less than the length of the message being sent.*/
    while (count < message->size) {
        //Gets encrypted character
        c = cipher(fgetc(message->file), fgetc(key->file));
        //Writes address of cipher character to pipe
        write(fd[WRITE], &c, sizeof(c));
        //Increments number of characters written.
        count++;
    }

    //Closes writing to pipe to end waiting by other child.
    close(fd[WRITE]);

}

/*Encrypts a message character (m) by using XOR with the value of a key
character (k)*/
int cipher(int m, int k) {
    return (m^k);
}

/** Reads data character by character from pipe and writes each character to output file
@destination - output file to be written to, @fd - pipe file descriptors for reading and writing.
*/
void secureReceive(FileStruct* destination, int fd[2]) {
    //Charcter
    int c;
    //Closes pipe for writing as child is reading from pipe.
    close(fd[WRITE]);
    //Reads characters from pipe and writes to file while read cotinues to wait for data.
    while(read(fd[READ], &c, sizeof(c))) writeOutput(destination->file, c);
    //Closes output file after all characters are read.
    fclose(destination->file);
    //Closes pipe for reading signalling end of communication with other process.
    close(fd[READ]);
}

/* Writes a cipher character (int) to file.
@output - file to write output to, @cipher - character to write to file.
*/
void writeOutput(FILE* output, int cipher) {
    //Writes integer character to file.
    fputc(cipher, output);
    //Flushes stdout buffer in case process is using stdout.
    fflush(stdout);
}
