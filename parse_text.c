/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
#include "parse_text.h"
#include "parser.h"
#include "list_utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024


int get_build_spec_list(BuildSpecList *specs, bool fflag, char *filename) {
    FILE **fp = open_makefile(fflag, filename);
    char file_line[MAX_LINE_LENGTH];        // Points to the incoming line
    bool isEnd = false;     // Flag set at the EOF mark so control loop knows when to end
    int lineNum = 0;        // Keeps track of line number for error messages
    /* Read line in, check if valid, parse the line into the specs of project */
    while (!isEnd) {
        lineNum++;
        get_file_line(*fp, &isEnd, lineNum, file_line);
        if (file_line == NULL) continue;
        parse_line(file_line, specs, lineNum);
    }
    fclose(*fp);
    free(fp);
    return 0;
}


char **tokenize(char *line, int *depsLen) {//, BuildSpecList *bsl) {
    /* Allocate a buffer for ind. words and one for the list. They both need
     * to be max length in case our line is one really long word */
    char **tokenList = calloc(MAX_LINE_LENGTH, sizeof(char *));
    char *buf = calloc(MAX_LINE_LENGTH, sizeof(char));
    char c;
    int tokenCount = 0;
    int stringLength = 0;
      
    for (int i = 0; i < MAX_LINE_LENGTH; i++) { 
        if (line[i] == '\t') continue;  // Avoids counting a tab as a token
        buf[stringLength] = line[i];    
        
        if (buf[stringLength] == '#') break;    // If we hit a comment, stop tokenizing
        
        /* Split the string on a space or at the end of the line, then put that in an array of words */
        if (buf[stringLength] == ' ' || buf[stringLength] == '\n' || buf[stringLength] == EOF) {
            if (line[i - 1] == ' ') continue;   // To avoid creating a token that is just a space
            c = buf[stringLength];      
            buf[stringLength++] = '\0'; // Good to give it a null terminator
            tokenList[tokenCount] = calloc(stringLength, sizeof(char));

            if (tokenList[tokenCount] == NULL) {
                fprintf(stderr, "Failed to allocate memory. Exiting...\n");
                exit(1);
            }
            
            if (strcpy(tokenList[tokenCount++], buf) == NULL) {
                fprintf(stderr, "Failed to copy string, exiting now...\n");
                exit(1);
            }

            /* If at the end of the line we want to leave */
            if (c == '\n' || c == EOF) break;
            stringLength = 0;
            continue;
        }
        stringLength++; 
    }
 
    *depsLen = tokenCount;
    if (buf != NULL) {
        free(buf);
    }
    return tokenList;
}

/**
 *  Checks if the string is just spaces and can be skipped
 */
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
        
        if (retStr == NULL) {
            fprintf(stderr, "Failure to allocate memory. Exiting...\n");
            exit(1);
        }

        for(int j = 0; j < i - 1; j++) {
            retStr[j] = cmdTolkens[index][j+1];
        }
        return retStr;
    }
    // make sure the correct number of things are removed from
    // cmdTolkens. 
}

/*
 * remove empty strings from the array
 */
char** cleanArgArray(char** uncleanArray, int len, int* resultLen) {
    char** clean = malloc(sizeof(char*) * len);
    for (int i=0; i < len; i++) {
    // make sure the NULL pointer is added correctly!
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

// Removes the colon if there is one
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
    int cmdsLen;
    char **tokens;
        
    if (c == '#') return;   // Ignore comments
    // starts with tab, must be a line of commands
    if (c == '\t') {
        BuildSpec *buildSpec = get_last_build_spec(buildSpecList);
        Command *cmd = malloc(sizeof(Command));
        
        if (cmd == NULL) {
            fprintf(stderr, "Error allocating memory. Exiting...\n");
            exit(1);
        }

        char** cmdTokens = tokenize(line, &cmdsLen);
        cmd->output = NULL;
        cmd->input = NULL;
        cmd->inputSet = 0;
        cmd->outputSet = 0;
        cmd->argv = cmdTokens;
        cmd->argv = cmdTokens;
        cmd->argc = cmdsLen;
        append_cmd_to_buildspec(buildSpec, cmd);
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
    }  // Checking to see if there is a colon to remove


    tokens = tokenize(line, &cmdsLen);//, );
    
    BuildSpec *buildSpec = malloc(sizeof(BuildSpec));
    
    if (buildSpec == NULL) {
        fprintf(stderr, "Failure to allocate memory. Exiting...\n");
        exit(1);
    }

    buildSpec->target = tokens[0];
    
    append_build_spec(buildSpecList, buildSpec);

    /* Shift each token over to get just dependencies */
    for (int i = 1; i < cmdsLen; i++) {
        tokens[i - 1] = tokens[i];
    }

    buildSpec->deps = tokens;
    tokens = NULL;
    buildSpec->depsLen = cmdsLen - 1;
    buildSpec->cmds = malloc(sizeof(CommandList));
    if (buildSpec->cmds == NULL) {
        fprintf(stderr, "Failure to allocate memory. Exiting...\n");
        exit(1);
    }
    buildSpec->cmds->len = 0;
}

void get_file_line(FILE *fp, bool *isEnd, int lineNum, char *input) {
    
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        input[i] = fgetc(fp);
        if (input[i] == '\n' || input[i] == EOF) {
            if (input[i] == EOF) *isEnd = true;
            return;
        }
        if (input[i] == '\0') {
            fprintf(stderr, "%d: Invalid Null char mid line\n", lineNum);
            exit(1);
        }
    }
    fprintf(stderr, "%d: Line too long, ignoring\n", lineNum);
    exit(1);
}

FILE **open_makefile(bool fflag, char *filename) {
    FILE **fptr = malloc(sizeof(FILE *));
    
    if (fptr == NULL) {
        fprintf(stderr, "Failure to allocate memory. Exiting...\n");
        exit(1);
    }

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
    /* IMPORTANT: This throws a warning in Clang but it is just because we
     * this isnt being freed. However it is just allocated once and is used all 
     * over the program so it would end up being more trouble and causeing more
     * potential bugs to free it near the end than it is to just leave it be */

    return fptr;                   
}

