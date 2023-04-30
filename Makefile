CFLAGS= -g -std=gnu99 -Wall -Werror

panascript: parser.o compiler.o main.o
	gcc $(CFLAGS) -o panascript main.o parser.o compiler.o

main.o: main.c shared.h
	gcc $(CFLAGS) -c main.c

parser.o: parser.c shared.h dfsa.h
	gcc $(CFLAGS) -c parser.c

compiler.o: parser.c shared.h
	gcc $(CFLAGS) -c compiler.c

dfsa_parser: dfsa_parser.o print_utils.o
	gcc $(CFLAGS) -o dfsa_parser dfsa_parser.o print_utils.o

dfsa_parser.o: dfsa_parser.c dfsa.h print_utils.h
	gcc $(CFLAGS) -c dfsa_parser.c

print_utils.o: print_utils.c print_utils.h
	gcc $(CFLAGS) -c print_utils.c
