CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = -lm
CC = gcc

obj = main.o

vpath %.h ../include/

all: $(obj)

%.o: %.c complex.h
		$(CC) -c $< $(CFLAGS) $(LFLAGS)

clean:
		rm -rf main *.out *.o


