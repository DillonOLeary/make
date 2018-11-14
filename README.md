# make
Program 3 for cs537
* Developers: Dillon O'Leary, Ezra Boley 

## Program Structure
There are three phases that the program goes through. 

First it reads each line of the file using the functions in parse\_text.c and compiles a list of targets, commands, and dependencies. 

Then it uses a hashmap and the functions in assess\_specs.c to create a graph of dependencies and that is where it decides which commands need to be run and which can be discarded.

Lastly the compiled list of instructions is passed into spawn.c which is responsible for creating each process and making sure each executes successfully.

make.c contains the main control loop for the program.

## Running
537make accepts a specific target to build as a command line arguement as well as the -f option followed by a specific file name.

## Memory Leaks
When using valgrind, users may notice memory leaks from tokenize, the spec list creator, or the hash map creator. These leaks are intentional because the memory allocated by those functions are used for most of the lifespan of the program.

## p3-1 Changes
All chages are shown in the p3-1\_changes.txt file. Notible changes are in the file assess\_specs.c in checkDeps, visitNode, getCommandList. These changes are meant to free the dummy nodes that are created when a file is checked for a dependency. Also the map pointer is passed in such to allocate less memory on the heap. A new module was also created called free\_str.h and free\_str.h. This module is used to free all the memory allocated in the nodes, build specs, and commands. In the hash\_map.c file, a flag was added to indicate if the node is a dummy node.
