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
            //if (stringLength == 0) continue;
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

char* getIOFilename(char** cmdTolkens, int index) {
    char* retStr;
    if (cmdTolkens[index][1] == '\0') {
        retStr = cmdTolkens[index + 1];
        cmdTolkens[index] = cmdTolkens[index + 1] = "";
        return retStr;
    } else {
        int i;
        for (i = 0; cmdTolkens[index][i] != '\0'; i++);
        retStr = malloc(sizeof(char) * i);
        for(int j = 0; j < i - 1; j++) {
            retStr[j] = cmdTolkens[index][j+1];
        }
        return retStr;
    }
    // FIXME make sure the correct number of things are removed from
    // cmdTolkens. maybe just make them balnk and then scan for blanks?
}

/*
 * remove empty strings from the array
 */
char** cleanArgArray(char** uncleanArray, int len, int* resultLen) {
    char** clean = malloc(sizeof(char*) * len);
    for (int i=0; i < len; i++) {
    // FIXME is this the correct way to clcean the array?
    // FIXME make sure the NULL pointer is added correctly!
        if (uncleanArray[i][0] != '\0') {
            clean[*resultLen++] = uncleanArray[i];
        } 
    }
}

/**
 * This will read through the command tolkens
 * and set the redirects and set the length 
 * equal to the size of the array without the
 * redirects. Will throw an error if the redirect
 * is attempted to be set twice
 */
void setRedirects(char** cmdTolkens, Command* cmd) {
    cmd->argc = 0;
    int numElem = 0;
    for (int i=0; cmdTolkens[i] != NULL; i++) {
        if (cmdTolkens[i][0] == '<') {
            // set in
            if (1 == cmd->inputSet) {
                fprintf(stderr, "Cannot set the input twice!\n");
                exit(-1);
            }
            cmd->inputSet == 1;
            char* filename = getIOFilename(cmdTolkens, i);
            FILE* infp;
            if(NULL == fopen(filename, "r")) {
                fprintf(stderr, "Error openinging file: \"%s\"", filename);
                exit(-1);
            }
        }
        if (cmdTolkens[i][0] == '>') {
            // set out
            if (1 == cmd->outputSet) {
                fprintf(stderr, "Cannot set the output twice!\n");
                exit(-1);
            }
            cmd->outputSet == 1;
            char* filename = getIOFilename(cmdTolkens, i);
            FILE* outfp;
            if(NULL == fopen(filename, "r")) {
                fprintf(stderr, "Error openinging file: \"%s\"", filename);
                exit(-1);
            }
        }
        numElem++;
    }
    cmd->argv = cleanArgArray(cmdTolkens, numElem, &(cmd->argc));
}

void parse_line(char *line, BuildSpecList *buildSpecList, int lineNum) {
    char c = line[0];
    int i;
    int cmdsLen;
    char **tokens;

    //FIXME this must return an error if there is a null char in the line
    // or if the line is too long    
    if (c == '#') return; 
    // Must be a line of commands
    if (c == '\t') {
        BuildSpec *buildSpec = get_last_build_spec(buildSpecList);
        Command *cmd = malloc(sizeof(Command));
        char** cmdTolkens = tokenize(line, &cmdsLen);
        cmd->output = stdout;
        cmd->input = stdin;
        cmd->inputSet = 0;
        cmd->outputSet = 0;
        cmd->argv = cmdTolkens;
        // FIXME make sure this next thing works
        //setRedirects(cmdTolkens, cmd);
        // cmd->argc = cmdsLen;
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
    } else {
        tokens[0][i - 1] = '\0';
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

