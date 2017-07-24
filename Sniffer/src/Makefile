CFLAGS = -g -O2 -Wall -pedantic -ansi
LFLAGS = -lpcap                                                   

vpath %.h ../include/                                                                   

main.o: main.c sniffer.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:                                                                                 
		rm -rf *.o
