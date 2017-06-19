IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lncurses -lmenu
CFLAGS = -O2 -Wall -pedantic -ansi
CC = gcc

objects = main.o manager.o list.o
SUBDIRS = $(LDIR) $(SDIR)

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: $(objects)
		$(CC) -o manager $< $(CFLAGS) $(LFLAGS) 

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR):$(LDIR)

clean:
		rm -rf manager *.o
