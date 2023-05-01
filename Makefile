CFLAGS= -g -std=gnu99 -Wall

panascript: parser.o compiler.o main.o dfsa_parser.o print_utils.o
	gcc $(CFLAGS) -o panascript main.o parser.o compiler.o dfsa_parser.o print_utils.o


main.o: main.c shared.h
	gcc $(CFLAGS) -c main.c

parser.o: parser.c shared.h dfsa.h
	gcc $(CFLAGS) -c parser.c

compiler.o: parser.c shared.h
	gcc $(CFLAGS) -c compiler.c


dfsa_parser.o: dfsa_parser.c dfsa.h print_utils.h
	gcc $(CFLAGS) -c dfsa_parser.c

print_utils.o: print_utils.c print_utils.h
	gcc $(CFLAGS) -c print_utils.c


test_dfsa_parser.o: test_dfsa_parser.c dfsa.h
	gcc $(CFLAGS) -c test_dfsa_parser.c

test_dfsa_parser: test_dfsa_parser.o dfsa_parser.o print_utils.o
	gcc $(CFLAGS) -o test_dfsa_parser test_dfsa_parser.o dfsa_parser.o print_utils.o
