IDIR = include
LDIR = lib
SDIR = src
PDIR = plugins
CFLAGS = -Wall -O2 -pedantic -ansi
LFLAGS = -lm -ldl
CC = gcc

objects = $(SDIR)/main.o $(LDIR)/complex.o
SUBDIRS = $(LDIR) $(SDIR) $(PDIR)

vpath %.c src:lib
vpath %.o src:lib

all: $(objects) 
		$(CC) -o Complex_calc $^ $(CFLAGS) $(LFLAGS)

objects: prepare

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
		cd plugins	&& $(MAKE) clean
		rm -rf Complex_calc *.o
	
