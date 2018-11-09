/**
 * authors: Dillon O'Leary
 * Ezra Boley
 */
/**
 * This is the main file for the
 * make program. It handles the high level function
 * calls
 */
#include "assess_specs.h"
#include "parser.h"
#include "parse_text.h"
#include "spawn.h"
#include "parse_args.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE * fp;
    BuildSpecList specs;
    char *root = NULL; // the root node. If it's not specified then
    // its assumed to be the first build spec
    // set root to be whatever is passed in by the command line
    // if there is something in the command then set it to that
    // otherwise it defaults to the first
    specs.len = 0;
    CommandList cmdList;
    char filename[MAX_ARG_LENGTH];
    bool fflag = false;
    if (argc == 4) {
        if (!strcmp(argv[1], "-f")) {
            root = argv[3];
        } else if (!strcmp(argv[2], "-f")) {
            root = argv[1];
        }
    } else if (argc == 2) {
        root = argv[1];
    }
    parse_args(argc, argv, &fflag, filename);
    fp = open_makefile(fflag, filename);
    getBuildSpecList(&specs, fp);
      
    if (root == NULL)
        specs.root = specs.frstBS->target;
    else specs.root = root;
    getCommandList(&cmdList, &specs);
    create_family(&cmdList);
}
