CFLAGS = -O2 -Wall -pedantic -ansi
LFLAGS = -lncurses

vpath %.h ../include/

main.o: main.c manager.h
		gcc -c $< $(CFLAGS) $(LFLAGS)

clean:
		rm -rf *.o
