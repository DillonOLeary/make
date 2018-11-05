#include <stdio.h>
#include <stdbool.h>
#include "parse_text.h"
#include "parser.h"
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("FORMAT: test_parse <file_to_test(e.g. test.make)>");
        exit(1);
    }
    FILE *mfp = open_makefile(true, argv[1]);
    BuildSpecList bsl;
    bsl.len = 0;
    getBuildSpecList(&bsl, mfp);
    int numErr = 0;
    if (strcmp(bsl.frstBS->target, "target1:")) {
        numErr++;
        printf("ERROR: first target != target1:, its %s\n", bsl.frstBS->target);
    }
    if (strcmp(bsl.frstBS->cmds->lstCmd->argv[0], "gcc")) {
        numErr++;
        printf("ERROR: first targets cmd != gcc, its %s\n", bsl.frstBS->cmds->lstCmd->argv[0]);
    }
    int i, j, k;
    for (i = 0; i < bsl.frstBS->cmds->frstCmd->argc; i++) {
        printf("%s ", bsl.frstBS->cmds->frstCmd->argv[i]);
    }
    printf("\nShould be: gcc -o file.txt file2.c \n");
    
    BuildSpec *currBuildSpec = bsl.frstBS;
    Command   *currCmd;
    for (i = 0; i < bsl.len; i++) {
        printf("BUILD SPEC #%d  || TARGET: %s   || NUM CMDS: %d   || NUM DEPS: %d \n", i, currBuildSpec->target, currBuildSpec->cmdlen, currBuildSpec->depsLen);
        for (j = 0; j < currBuildSpec->depsLen; j++) {
            printf("\tDEP #%d: %s\n", j, currBuildSpec->deps[j]);
        }
        if (currBuildSpec->cmdlen >= 1) currCmd = currBuildSpec->cmds->frstCmd;
        for (j = 0; j < currBuildSpec->cmdlen; j++) {
            printf("\tCMD #%d\n", j);
            for (k = 0; k < currCmd->argc; k++) {
                printf("\t\tARGV[%d] == %s\n",k, currCmd->argv[k]);
            }
            currCmd = currCmd->nxtCmd;
        }
        currBuildSpec = currBuildSpec->nxtBuildSpec;
    }

    if (numErr == 0) printf("SUCCESS, ALL TESTS PASSED\n");
    else printf("NUM ERRORS: %d\n", numErr);
//    printf("ARG[0]: %s\n", bsl.lstBS->target);

}
