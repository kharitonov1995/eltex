CFLAGS = -g -O2 -Wall -pedantic -ansi 
LFLAGS = -lpthread

vpath %.h ../include/
		
service.o: service.c service.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
