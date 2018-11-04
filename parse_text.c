#include "parse_text.h"
#include "parser.h"
#include "list_utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512


int getBuildSpecList(BuildSpecList *specs, FILE *fp) {
    char *file_line;
    bool isEnd = false;
    while (!isEnd) {
        file_line = get_file_line(fp, &isEnd);
        parse_line(file_line, specs);
    }

    return 0;
}

char **tokenize(char *line, int *depsLen) {
    /* Allocate a buffer for ind. words and one for the list. They both need
     * to be max length in case our line is one really long word */
    char **tokenList = calloc(MAX_LINE_LENGTH, sizeof(char *));
    char *buf = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
    char c;
    int i;
    int tokenCount = 0;
    int stringLength = 0;
    
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


void parse_line(char *line, BuildSpecList *buildSpecList) {
    char c = line[0];
    int i;
    int cmdsLen;
    char **tokens;
    if (c == '#') return; 
    // Must be a line of commands
    if (c == '\t') {
        BuildSpec *buildSpec = get_last_build_spec(buildSpecList);
        Command *cmd = malloc(sizeof(Command));
        cmd->args = tokenize(line, &cmdsLen);
        cmd->argc = cmdsLen;
        buildSpec->cmdlen++;
        append_cmd(buildSpec, cmd);
        return;
    }
    tokens = tokenize(line, &cmdsLen);
    BuildSpec *buildSpec = malloc(sizeof(BuildSpec));
    buildSpec->target = tokens[0];
    append_build_spec(buildSpecList, buildSpec);
    for (i = 1; i < cmdsLen; i++) {
        tokens[i - 1] = tokens[i];
    }
    buildSpec->deps = tokens;
    buildSpec->depsLen = cmdsLen - 1;
    buildSpec->cmds = malloc(sizeof(CommandList));
    buildSpec->cmds->len = 0;
    printf("here2");
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

