#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include "parse_args.h"

void parse_args(int argc, char *argv[], bool *fflag, char *filename, char* root) {
    char option;

    while ((option = getopt(argc, argv, "f:")) != -1) {
        switch (option) {
            case 'f':
                for (int i = 0; i < MAX_ARG_LENGTH; i++) {
                    filename[i] = optarg[i];
                    if (filename[i] == '\0') { 
                        *fflag = true;
                        break;
                    }
                }
                if (!*fflag) 
                    printf("ERROR File path too long, reverting to standard makefile name search\n");
                break;
            
            default:
                printf("Unrecognized command line arg, exiting...\n");
                exit(1);
        }

    }
}
