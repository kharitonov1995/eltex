IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lncurses
CFLAGS = -O2 -Wall -pedantic -ansi

CC = gcc

objects = $(SDIR)/main.o $(LDIR)/editor.o
SUBDIRS = $(LDIR) $(SDIR)

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: $(objects)
		$(CC) -o editor $^ $(CFLAGS) $(LFLAGS)

$(objects): prepare

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR):$(LDIR)

clean:
		cd src 	&& $(MAKE) clean
		cd lib	&& $(MAKE) clean	
		rm -rf *.o editor
