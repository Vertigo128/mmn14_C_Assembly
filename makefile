assembler: assembler.c assembler.h data.h errors.h parsing.o printing.o tables.o helpers.o files.o
	gcc -g -ansi -Wall -pedantic assembler.c parsing.o printing.o tables.o helpers.o files.o -o assembler

parsing.o: parsing.c parsing.h data.h consts.h errors.h helpers.o printing.o 
	gcc -c -ansi -Wall -pedantic parsing.c -o parsing.o

printing.o: printing.c printing.h consts.h data.h  errors.h
	gcc -c -ansi -Wall -pedantic printing.c -o printing.o

tables.o: tables.c tables.h consts.h data.h  errors.h 
	gcc -c -ansi -Wall -pedantic tables.c -o tables.o

helpers.o: helpers.c helpers.h consts.h data.h  errors.h 
	gcc -c -ansi -Wall -pedantic helpers.c -o helpers.o

files.o: files.c files.h consts.h data.h  errors.h 
	gcc -c -ansi -Wall -pedantic files.c -o files.o
