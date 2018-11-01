test_assess_specs: test_create_specs.c assess_specs.c assess_specs.h test_create_specs.h parser.h hash_map.c test_hash.c test_assess_specs.c
	 gcc -g -Wall -Wextra -o test_assess_specs test_create_specs.c test_create_specs.h parser.h hash_map.c test_assess_specs.c assess_specs.c assess_specs.h 

test_hash: parser.h hash_map.c test_hash.c 
	gcc -g -Wall -Wextra -o test_hash test_create_specs.c test_create_specs.h parser.h hash_map.c test_hash.c 

clean:
	rm test_hash
	rm test_assess_specs
