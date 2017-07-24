CFLAGS = -g -O2 -Wall -pedantic -ansi 
LFLAGS = 

vpath %.h ../include/
		
sniffer.o: sniffer.c sniffer.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
