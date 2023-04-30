CFLAGS= -g -std=gnu99 -Wall -Werror

panascript: parser.o compiler.o main.o
	gcc $(CFLAGS) -o panascript main.o parser.o compiler.o

main.o: main.c shared.h
	gcc $(CFLAGS) -c main.c

parser.o: parser.c shared.h
	gcc $(CFLAGS) -c parser.c

compiler.o: parser.c shared.h
	gcc $(CFLAGS) -c compiler.c
