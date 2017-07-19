CFLAGS = -g -O2 -Wall -pedantic -ansi
LFLAGS =                                                        

vpath %.h ../include/                                                                   

server.o: server.c broadcast.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 
  
client.o: client.c broadcast.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:                                                                                 
		rm -rf *.o
