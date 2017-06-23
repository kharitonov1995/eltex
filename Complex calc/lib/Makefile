CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = -lm
CC = gcc

obj = complex.o

vpath %.h ../include/

all: $(obj)

%.o: %.c complex.h
		$(CC) -fPIC -c $< $(CFLAGS) $(LFLAGS)
		
clean:
		rm -rf *.o
