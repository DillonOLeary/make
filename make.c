/**
 * This is the main file for the
 * make program. It handles the high level function
 * calls
 */
#include "assess_specs.h"
#include "parser.h"
#include "parse_text.h"
#include "spawn.h"

int main() {
    FILE * fp;
    BuildSpecList specs;
    // FIXME set root to be whatever is passed in by the command line
    // if there is something in the command then set it to that
    // otherwise it defaults to the first
    specs.len = 0;
    CommandList cmdList;
    // TODO read in the args
    fp = open_makefile(1, "test.make5");
    getBuildSpecList(&specs, fp);
    specs.root = specs.frstBS->target;
    getCommandList(&cmdList, &specs);
    create_family(&cmdList);
}
