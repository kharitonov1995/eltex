CFLAGS = -g -O2 -Wall -pedantic -ansi
LFLAGS =                                                                    

vpath %.h ../include/                                                                   

main.o: main.c service.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 
  
client.o: client.c service.h                                                                
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:                                                                                 
		rm -rf main *.o *.out
