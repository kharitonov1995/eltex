CFLAGS = -O2 -Wall -pedantic -ansi
LIBS = -lncurses

vpath %.h ../include/

main.o: main.c manager.h
		gcc -c $(CFLAGS) $< $(LIBS)
