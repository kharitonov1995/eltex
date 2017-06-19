LFLAGS = -lncurses
CFLAGS = -O2 -Wall -pedantic -ansi

vpath %.h ../include/

main.o: main.c editor.h
		gcc -c $< $(CFLAGS) $(LFLAGS)
