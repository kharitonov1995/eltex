CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = 
CC = gcc

obj = malloc.o

vpath %.h ../include/

all: $(obj)

%.o: %.c complex.h
		$(CC) -c $< $(CFLAGS) $(LFLAGS)
		
clean:
		rm -rf *.o
