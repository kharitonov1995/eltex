CFLAGS = -g -O2 -Wall -pedantic -ansi 
LFLAGS = 

vpath %.h ../include/
		
handlers.o: service.c service.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
