LFLAGS = -lncurses
CFLAGS = -O2 -Wall -pedantic -ansi

vpath %.h ../include/
		
editor.o: editor.c editor.h
		gcc -c $< $(CFLAGS) $(LFLAGS)
