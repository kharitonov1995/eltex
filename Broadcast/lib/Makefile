CFLAGS = -g -O2 -Wall -pedantic -ansi 
LFLAGS = 

vpath %.h ../include/
		
broadcast.o: broadcast.c broadcast.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
