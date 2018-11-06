/**
 * This is the main file for the
 * make program. It handles the high level function
 * calls
 */
#include "assess_specs.h"
#include "parser.h"
#include "parse_text.h"

int main() {
    FILE * fp;
    BuildSpecList specs;
    // FIXME set root to be whatever is passed in by the command line
    specs.len = 0;
    CommandList cmdList;
    // TODO read in the args
    fp = open_makefile(1, "test.make3");
    getBuildSpecList(&specs, fp);
    getCommandList(&cmdList, &specs);
}
