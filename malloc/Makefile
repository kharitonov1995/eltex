IDIR = include
LDIR = lib
SDIR = src

CC = gcc

objects = $(SDIR)/main.o $(LDIR)/malloc.o
SUBDIRS = $(LDIR) $(SDIR) $(PDIR)

vpath %.c src:lib
vpath %.o src:lib

all: $(objects) 
		$(CC) -o malloc $^

$(objects): prepare

.PHONY: prepare $(SUBDIRS)

prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@

src : lib

clean:
		cd src 		&& $(MAKE) clean
		cd lib		&& $(MAKE) clean
		cd include	&& $(MAKE) clean
		rm -rf malloc *.o latex html
	
