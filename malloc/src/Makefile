CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = 
CC = gcc

obj = main.o

vpath %.h ../include/

all: $(obj)

%.o: %.c malloc.h
		$(CC) -c $< $(CFLAGS) $(LFLAGS)

clean:
		rm -rf main *.o *out


