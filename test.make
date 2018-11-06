target1: targ2 targ3 t4
	gcc -o file.txt file2.c

targ2: targ3.o
	gcc -opt file4 
   
  
#heres a comment
targ3.o: make me a file
 invalid!
	valid command
  
t4:
	cmd1 is good
	cmd2 is better
	cmd3 is best

targ3:    
	should be empty


