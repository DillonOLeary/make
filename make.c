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



int main(int argc, char *argv[]) {
    FILE * fp;
    BuildSpecList specs;
    char* root = NULL; // the root node. If it's not specified then
    // its assumed to be the first build spec
    // FIXME set root to be whatever is passed in by the command line
    // if there is something in the command then set it to that
    // otherwise it defaults to the first
    specs.len = 0;
    CommandList cmdList;
    // TODO read in the args
    //
    //
    char filename[MAX_ARG_LENGTH];
    bool fflag = false;
    parse_args(argc, argv, &fflag, filename, root);
    fp = open_makefile(fflag, filename);
    getBuildSpecList(&specs, fp);
    specs.root = specs.frstBS->target;
    printf("ROOT: %s\n", specs.root);
    getCommandList(&cmdList, &specs);
    create_family(&cmdList);
}
