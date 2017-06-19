IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lncurses
CFLAGS = -O2 -Wall -pedantic -ansi

CC = gcc

objects = main.o editor.o
SUBDIRS = $(LDIR) $(SDIR)

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: $(objects)
		$(CC) -o editor $< $(CFLAGS) $(LFLAGS)

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR):$(LDIR)

clean:
	rm -rf *.o
