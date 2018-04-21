#include <sys/types.h>
#include <stdio.h>
#include <unistd.h> /* POSIX requires getopt (). */
/* **
* A getopt () example . More info may be found at the man page :
* ~> man 3 getopt
* Try this program with any valid or invalid arguments :
* ~> ./ < your_executable_name > -a -b -c
* ~> ./ < your_executable_name > -x -a -z
* ~> ./ < your_executable_name > -a -b hello
*/
int main ( int argc , char * argv []) {
    int opt;
    FILE *inputFile, *outputFile, *keyFile;

    while ((opt = getopt(argc, argv, "ik:o")) != -1) {
        switch (opt) {
            case 'i':
                inputFile = optarg;
                printf("Option: '%c'\n", opt);
                break;
            case 'o':
                outputFile = optarg;
                printf("Option: '%c'\n", opt);
                break;
            case 'k':
                keyFile = optarg;
                printf("Option: '%c' w/ mandatory Argument: \"%s\"\n", opt, optarg);
                break;
            case ':':
            case '?':
            default:
                printf("Invalid option or missing argument: '-%c'.\n", optopt);
                break;
        }
    }

    printf("%d\n", argc);

    return 0;
}
