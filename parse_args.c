#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include "parse_args.h"
#include <string.h>

void parse_args(int argc, char *argv[], bool *fflag, char *filename, char* root) {
    /*if (argc == 4) {
        // There is a target in there somewhere
        printf("arg[0]: %s\n", argv[0]);
        char *strToCopy;
        if (!strcmp(argv[1], "-f")) {
            strToCopy = argv[3];
            printf("CASE 1!\n");
        } else if (!strcmp(argv[2], "-f")) {
            strToCopy = argv[1];
            printf("CASE 2!\n");
        }
        printf("STRING TO COPY: %s\n", strToCopy);
        if (strToCopy != NULL) {
            for (int i = 0; i < MAX_ARG_LENGTH; i++) {
                root[i] = strToCopy[i];
                if (root[i] == '\0') {
                    break;
                }
            }
        }

    }*/
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
            case '?':
                printf("WOW AN ARG?!\n");
                exit(1);
                break;
            default:
                printf("Unrecognized command line arg, exiting...\n");
                exit(1);
        }

    }
}
