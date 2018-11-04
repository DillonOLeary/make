#include "parse_text.h"
#include "parser.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512


char **tokenize(char *line, int *depsLen) {
    /* Allocate a buffer for ind. words and one for the list. They both need
     * to be max length in case our line is one really long word */
    char **tokenList = calloc(MAX_LINE_LENGTH, sizeof(char *));
    char *buf = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
    char c;
    int i;
    int tokenCount = 0;
    int stringLength = 0;
    
    for (i = 0; line[i] != '\n' && line[i] != EOF; i++){ 
        printf("%c",line[i]);
    }

    printf("\n");

    for (i = 0; i < MAX_LINE_LENGTH; i++) { 
        buf[stringLength] = line[i];
        if (buf[stringLength] == ' ' || buf[stringLength] == '\n' || buf[stringLength] == EOF) {
            c = buf[stringLength]; 
            buf[stringLength++] = '\0'; // Probably good to give it a null terminator
            tokenList[tokenCount] = calloc(stringLength, sizeof(char));
            if (strcpy(tokenList[tokenCount++], buf) == NULL) return NULL;
            if (c == '\n' || c == EOF) break;
            stringLength = 0;
            continue;
        }
        stringLength++; 
        //printf("char: %c\n", buf[stringLength - 1]);  // Little bit of debug
    }

    for (i = 0; i < tokenCount; i++) {
        printf("token %d: %s\n", i, tokenList[i]);
    }

    return tokenList;
}


void parse_line(char *line, BuildSpecList *buildSpecList, int currBSindex, int currCmdIndex) {
    char c = line[0];
    int i;
    int cmdsLen;
    char **tokens;
    if (c == '#') return; 
    // Must be a line of commands
    //if (c == '\t') {
        //buildSpecList->list[currBuildSpec]->cmds[currCmdIndex] = ;
        //buildSpecList->list[currBuildSpec]->cmds[currCmdIndex]->args = tokenize(line, &cmdsLen);
        //buildSpec->cmdsLen += cmdsLen;
        //return;
    //}
    
    //currWordType = command;
    tokens = tokenize(line, &cmdsLen);

    for (i = 0; i < MAX_LINE_LENGTH; i++) {
        if (c == '\0') {
            break;
        }
        
        if (c == ':') {

            buildSpecList->list[currBSindex]->target = tokens[0];
            
            break;
        }
    }

    free(line);
}

char *get_file_line(FILE *fp, bool *isEnd) {
    char *input = malloc(MAX_LINE_LENGTH * sizeof(char));
    int i;
    char c;
    for (i = 0; i < MAX_LINE_LENGTH; i++) {
        input[i] = fgetc(fp);
        if (input[i] == '\n' || input[i] == EOF) {
            if (input[i] == EOF) *isEnd = true;
            return input;
        }
    }
}



FILE *open_makefile(bool fflag, char *filename) {
    FILE **fptr = malloc(sizeof(FILE *));
    
    if (fflag) {
        *fptr = fopen(filename, "r");
        if (*fptr == NULL) {
            fprintf(stderr, "Specified file not found, exiting now\n");
            exit(1);
        }
    } else {
        *fptr = fopen("makefile", "r");
        if (*fptr == NULL) {
            // No makefile found, moving on
            *fptr = fopen("Makefile", "r");
            if (*fptr == NULL) {
                // Neither makefile nor Makefile were found, must exit
                fprintf(stderr, "No file named makefile or Makefile found, exiting now\n");
                exit(1);
            }
        }
    }

    return *fptr;
}

