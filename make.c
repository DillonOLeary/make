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
#include "hash_map.h"
#include "parse_text.h"
#include "spawn.h"
#include "parse_args.h"
#include "free_str.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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
    get_build_spec_list(&specs, fflag, filename);
    if (root == NULL)
        specs.root = specs.frstBS->target;
    else specs.root = root;
    DAG_map map;
    getCommandList(&map, &cmdList, &specs);
    BuildSpec *curr_bs = specs.frstBS;
    BuildSpec *prev_bs;
    Command   *curr_cmd = curr_bs->cmds->frstCmd;
    Command   *prev_cmd;
    // FREE THE LIST!
/*    
    for (int i = 0; i < specs.len; i++) {
        for (int j = 0; j < curr_bs->cmds->len; j++) {
            printf("argc: %s\n", curr_bs->cmds->frstCmd->nxtCmd->argv[0]);
            for (int k = 0; k < curr_cmd->argc; k++) {
                printf("CURR CMD: %d\n", curr_cmd->argv[k]);
                //if (curr_cmd->argv[k] != NULL && curr_cmd->argv[k] != '\0')
                free(curr_cmd->argv[k]);
                printf("HERE0\n");
            }
            //if (curr_cmd != NULL && curr_cmd->argv != NULL) free(curr_cmd->argv);
            printf("here1\n");
            prev_cmd = curr_cmd;
            curr_cmd = curr_cmd->nxtCmd;
            if (prev_cmd != NULL) free(prev_cmd);
            printf("here2\n");
        }
        // free(curr_bs->cmds);
        printf("here3\n");
        prev_bs = curr_bs;
        curr_bs = curr_bs->nxtBuildSpec;
        // free(prev_bs);
        printf("here4\n");
    }
    printf("DONE\n");
  */  
    create_family(&cmdList);
    //freeHashMap(&map);
}
