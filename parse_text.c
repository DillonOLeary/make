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
    int lineNum = 1;
    while (!isEnd) {
        file_line = get_file_line(fp, &isEnd);
        parse_line(file_line, specs, lineNum++);
    }
    return 0;
}


char **tokenize(char *line, int *depsLen) {
    /* Allocate a buffer for ind. words and one for the list. They both need
     * to be max length in case our line is one really long word */
    char **tokenList = calloc(MAX_LINE_LENGTH, sizeof(char *));
    char *buf = calloc(MAX_LINE_LENGTH, sizeof(char));
    char c;
    int i;
    int tokenCount = 0;
    int stringLength = 0;

    for (i = 0; i < MAX_LINE_LENGTH; i++) { 
        if (line[i] == '\t') continue;
        buf[stringLength] = line[i];
                if (buf[stringLength] == '#') break;
        if (buf[stringLength] == ' ' || buf[stringLength] == '\n' || buf[stringLength] == EOF) {
            if (line[i - 1] == ' ') continue;
            c = buf[stringLength]; 
            buf[stringLength++] = '\0'; // Probably good to give it a null terminator
            tokenList[tokenCount] = calloc(stringLength, sizeof(char));
            
            if (strcpy(tokenList[tokenCount++], buf) == NULL) {
                printf("Failed to copy string, exiting now...\n");
                exit(1);
            }

            if (c == '\n' || c == EOF) break;
            stringLength = 0;
            continue;
        }
        stringLength++; 
    }

//    for (i = 0; i < tokenCount; i++) {
//        printf("token %d: %s\n", i, tokenList[i]);
//    }
    *depsLen = tokenCount;
    return tokenList;
}

bool is_empty(char *line) {
    for (int i = 0; line[i] != '\n' && line[i] != EOF; i++) {
        if (line[i] != ' ') return false;
    }
    return true;
}

void parse_line(char *line, BuildSpecList *buildSpecList, int lineNum) {
    char c = line[0];
    int i;
    int cmdsLen;
    char **tokens;
    
    if (c == '#') return; 
    // Must be a line of commands
    if (c == '\t') {
        BuildSpec *buildSpec = get_last_build_spec(buildSpecList);
        Command *cmd = malloc(sizeof(Command));
        cmd->argv = tokenize(line, &cmdsLen);
        cmd->argc = cmdsLen;
        append_cmd_to_buildspec(buildSpec, cmd);
        return;
    }
    if (c == '\n' || c == EOF) return;  // Need a further check
    if (c == ' ') {
        if (!is_empty(line)) {
            printf("Error line:%d has invalid spacing\n", lineNum);
            exit(1);
        }
    }
    tokens = tokenize(line, &cmdsLen);
    
    BuildSpec *buildSpec = malloc(sizeof(BuildSpec));
    buildSpec->target = tokens[0];
    for (i = 0; '\0' != tokens[0][i]; i++);
    if (tokens[0][i - 1] != ':') {
    // Check if its a valid target
        free(buildSpec);
        return;
    }
    append_build_spec(buildSpecList, buildSpec);
    for (i = 1; i < cmdsLen; i++) {
        tokens[i - 1] = tokens[i];
    }
    buildSpec->deps = tokens;
    buildSpec->depsLen = cmdsLen - 1;
    buildSpec->cmds = malloc(sizeof(CommandList));
    buildSpec->cmds->len = 0;
    free(line);
}

char *get_file_line(FILE *fp, bool *isEnd) {
    char *input = malloc(MAX_LINE_LENGTH * sizeof(char));
    int i;

    for (i = 0; i < MAX_LINE_LENGTH; i++) {
        input[i] = fgetc(fp);
        if (input[i] == '\n' || input[i] == EOF) {
            if (input[i] == EOF) *isEnd = true;
            return input;
        }
    }
    return "";
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

