CC = gcc
WARNING_FLAGS = -g -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

make: make.o assess_specs.o hash_map.o spawn.o parse_text.o list_utils.o parse_args.o
	$(CC) -o $(EXE) make.o assess_specs.o spawn.o parser.h hash_map.o parse_text.o list_utils.o parse_args.o 

make.o: make.c
	$(CC) $(WARNING_FLAGS) -c make.c

assess_specs.o: assess_specs.c assess_specs.h
	$(CC) $(WARNING_FLAGS) -c assess_specs.c

parser.o: parser.h parser.c
	$(CC) $(WARNING_FLAGS) -c parser.c

hash_map.o: hash_map.c hash_map.h
	$(CC) $(WARNING_FLAGS) -c hash_map.c

parse_text.o: parse_text.h parse_text.c
	$(CC) $(WARNING_FLAGS) -c parse_text.c

list_utils.o: list_utils.c list_utils.h
	$(CC) $(WARNING_FLAGS) -c list_utils.c

spawn.o: spawn.h spawn.c
	$(CC) $(WARNING_FLAGS) -c spawn.c

parse_args.o: parse_args.c parse_args.h
	$(CC) $(WARNING_FLAGS) -c parse_args.c
	
make:assess_specs.c assess_specs.h parser.h hash_map.c test_hash.c parse_text.c parse_text.h list_utils.c list_utils.h make.c

assess_specs.o:

test_assess_specs: test_create_specs.c assess_specs.c assess_specs.h test_create_specs.h parser.h hash_map.c test_hash.c test_assess_specs.c
	 gcc -g -Wall -Wextra -o test_assess_specs test_create_specs.c test_create_specs.h parser.h hash_map.c test_assess_specs.c assess_specs.c assess_specs.h 

test_hash: parser.h hash_map.c test_hash.c 
	gcc -g -Wall -Wextra -o test_hash test_create_specs.c test_create_specs.h parser.h hash_map.c test_hash.c 

test_parse: parser.h parse_text.h parse_text.c list_utils.c list_utils.h parse_test.c
	gcc -g -Wall -Wextra -o test_parse parse_text.c list_utils.c parse_test.c

clean:
	rm *.o
	rm test_assess_specs
	rm test_parse
	rm test_hash
