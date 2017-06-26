CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = -lm
CC = gcc

dlibs = libadd.so libdivision.so libmul.so libsub.so

vpath %.h ../include/

all: $(dlibs)

lib%.so: %.o
		$(CC) -shared $< -o $@ $(CFLAGS) $(LFLAGS)
		
%.o: %.c complex.h
		$(CC) -c $^ $(CFLAGS) $(LFLAGS)

clean:
		rm -rf *.o *.so
