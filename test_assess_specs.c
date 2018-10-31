#include "test_create_specs.h"
#include "assess_specs.h"
#include "parser.h"

int main() {
    BuildSpecList specs;
    createSpecList(&specs);
    CommandList cmdList;
    getCommandList(&cmdList, &specs);
}
