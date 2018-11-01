#include "parse_text.h"

#define MAX_LINE_LENGTH
#define MAX_WORD_LENGTH

enum token_t{target, command, dependency};

char **tokenize(char *line, int *depsLen) {
    /* Allocate a buffer for ind. words and one for the list. They both need
     * to be max length in case our line is one really long word */
    char **tokenList = calloc(MAX_LINE_LENGTH, sizeof(char *));
    char *buf = calloc(MAX_LINE_LENGTH, sizeof(char));
    int i;
    int tokenCount = 0;
    int stringLength = 0;

    for (i = 0; i < MAX_LINE_LENGTH; i++) { 
        if (line[i] == ' ' || line[i] == '\n') {
            buf[stringLength++] = '\0'; // Probably good to give it a null terminator
            tokenList[tokenCount] = calloc(stringLength, sizeof(char));
            if (strcpy(tokenList[tokenCount++], buf) == NULL) return NULL;
            stringLength = 0;
            continue;
        } 
        buf[stringLength++] = line[i];
printf("char: %c", buf[stringLength - 1]);  // Little bit of debug
    }

    return tokenList;
}


void parse_line(char *line, BuildSpecList *buildSpecList) {
    char c = line[0];
    enum token_t currWordType;
    int i;
    int cmdsLen;
    Command cmd;
    BuildSpec buildSpec = malloc(sizeof(BuildSpec));
  
    // Must be a line of commands
    if (c == '\t') {
        cmd->args = tokenize(line, &cmdsLen);
        buildSpec->cmdsLen += cmdsLen;
        
        return 0;
    }
    
    //currWordType = command;
    

    for (i = 0; i < MAX_LINE_LENGTH; i++) {
        if (c == '\0') {
            break;
        }
        
        if (c == ':') {
            currWordType = target;
        }
    }
}

char get_file_line() {

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

    return fptr;
}

