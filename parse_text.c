#include "parse_text.h"
#include "parser.h"
#include "list_utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024


int getBuildSpecList(BuildSpecList *specs, FILE *fp) {
    char *file_line;
    bool isEnd = false;
    int lineNum = 0;
    while (!isEnd) {
        lineNum++;
        file_line = get_file_line(fp, &isEnd, lineNum);
        if (file_line == NULL) continue;
        parse_line(file_line, specs, lineNum);
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

    /* DEBUG */
    //for (i = 0; i < tokenCount; i++) {
    //    printf("token %d: %s\n", i, tokenList[i]);
    //}
    
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
            clean[(*resultLen)++] = uncleanArray[i];
        } 
    }
    return clean;
}

/**
 * This will read through the command tolkens
 * and set the redirects and set the length 
 * equal to the size of the array without the
 * redirects. Will throw an error if the redirect
 * is attempted to be set twice
 */
/*
void setRedirects(char** cmdTokens, Command* cmd) {
    cmd->argc = 0;
    int numElem = 0;
    for (int i=0; cmdTokens[i] != NULL; i++) {
        if (cmdTokens[i][0] == '<') {
            // set in
            if (1 == cmd->inputSet) {
                fprintf(stderr, "Cannot set the input twice!\n");
                exit(-1);
            }
            cmd->inputSet == 1;
            char* filename = getIOFilename(cmdTokens, i);
            FILE* infp;
            if(NULL == fopen(filename, "r")) {
                fprintf(stderr, "Error openinging file: \"%s\"", filename);
                exit(-1);
            }
        }
        if (cmdTokens[i][0] == '>') {
            // set out
            if (1 == cmd->outputSet) {
                fprintf(stderr, "Cannot set the output twice!\n");
                exit(-1);
            }
            cmd->outputSet == 1;
            char* filename = getIOFilename(cmdTokens, i);
            FILE* outfp;
            if(NULL == fopen(filename, "r")) {
                fprintf(stderr, "Error openinging file: \"%s\"", filename);
                exit(-1);
            }
        }
        numElem++;
    }
    cmd->argv = cleanArgArray(cmdTokens, numElem, &(cmd->argc));
}
*/


/** DEPRECATED!
void flatten_tokens(char **tokens, int argc, char *flatTokens) {
    int cnt = 0;
    for (int i = 0; i < argc; i++) {
        for (int j = 0; tokens[i][j] != '\0'; j++) {
            flatTokens[cnt++] = tokens[i][j];
        }
        flatTokens[cnt++] = ' ';
    }
    flatTokens[cnt++] = '\n';
}

int check_target(char **tokens, int *argc) {
    char flattenBuf[MAX_LINE_LENGTH];

    for (int i = 0; i < *argc; i++) {
        for (int j = 0; tokens[i][j] != '\0'; j++) {
            if (tokens[i][j] == ':') {
                if (j == 0) {
                    // Remove ":" from list
                    for (int k = i; k < *argc - 1; k++) {
                        tokens[k] = tokens[k + 1];
                    }
                    *argc = *argc - 1;    
                } else {
                    // Remove ':' from string
                    if (tokens[i][j + 1] == '\0') {
                        tokens[i][j] = '\0';
                    } else {
                        tokens[i][j] = ' ';
                        flatten_tokens(tokens, *argc, flattenBuf);
                        printf("FLATTENED BUF: %s\n", flattenBuf);
                        tokens = tokenize(flattenBuf, argc);    // Its just crazy enough to work
                        printf("ARGC: %d\n", *argc);
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

*/


// This works instead of all of the above code haha
int check_colon(char *line) {
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        if (line[i] == ':') {
            line[i] = ' ';
            return 0;
        }
    }
    return 1;
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
        char** cmdTokens = tokenize(line, &cmdsLen);
        cmd->output = NULL;
        cmd->input = NULL;
        cmd->inputSet = 0;
        cmd->outputSet = 0;
        cmd->argv = cmdTokens;
        // FIXME make sure this next thing works
        //setRedirects(cmdTokens, cmd);
        //printf("The first arg is %s\n", cmd->argv[0]);
        cmd->argv = cmdTokens;
        cmd->argc = cmdsLen;
        append_cmd_to_buildspec(buildSpec, cmd);
        free(line);
        return;
    }
    if (c == '\n' || c == EOF) return;  // Need a further check
    if (c == ' ') {
        if (!is_empty(line)) {
            fprintf(stderr, "Error line:%d has invalid spacing\n", lineNum);
            exit(1);
        } else {
            return;
        }
    }

    if (check_colon(line)) {
        fprintf(stderr, "%d: Invalid line, neither a target nor a command\n", lineNum);
        exit(1);
    }  // Add checking to see if there is a colon

    tokens = tokenize(line, &cmdsLen);
    
    BuildSpec *buildSpec = malloc(sizeof(BuildSpec));
    
    buildSpec->target = tokens[0];
  
  // DEBUG  
//    for (i = 0; i < cmdsLen; i++) {
//        printf("TOKEN %d: %s\n", i, tokens[i]); 
//    }

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

char *get_file_line(FILE *fp, bool *isEnd, int lineNum) {
    char *input = malloc(MAX_LINE_LENGTH * sizeof(char));
   
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        input[i] = fgetc(fp);
        if (input[i] == '\n' || input[i] == EOF) {
            if (input[i] == EOF) *isEnd = true;
            return input;
        }
        if (input[i] == '\0') {
            fprintf(stderr, "%d: Invalid Null char mid line\n", lineNum);
            exit(1);
        }
    }
    fprintf(stderr, "%d: Line too long, ignoring\n", lineNum);
    exit(1);
    return NULL;
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

