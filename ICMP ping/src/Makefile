CFLAGS = -g -O2 -Wall -pedantic -ansi
LFLAGS =                                                  

vpath %.h ../include/                                                                   

main.o: main.c icmp.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:                                                                                 
		rm -rf *.o
