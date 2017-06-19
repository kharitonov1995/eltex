CFLAGS = -O2 -Wall -pedantic -ansi 
LFLAGS = -lncurses

vpath %.h ../include/

list.o: list.c list.h
		gcc -c $< $(CFLAGS)
		
manager.o: manager.c manager.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
