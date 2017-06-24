CFLAGS = -O2 -Wall -pedantic
LFLAGS = -lncurses

vpath %.h ../include/

main.o: main.c manager.h
		gcc -c $< $(CFLAGS) $(LFLAGS)

clean:
		rm -rf main *.o *.out
