CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = -lm
CC = gcc

dlibs = libadd.so libdiv.so libmul.so libsub.so

vpath %.h ../include/

all: $(dlibs)

lib%.so: p_%.o
		$(CC) -shared $< -o $@ $(CFLAGS) $(LFLAGS)
		
p_%.o: p_%.c complex.h
		$(CC) -c $^ $(CFLAGS) $(LFLAGS)

p_%.c: %.c

clean:
		rm -rf *.o *.so
