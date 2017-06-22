IDIR = include
LDIR = lib
SDIR = src
PDIR = plugins

CC = gcc

objects = main.o
SUBDIRS = $(LDIR) $(SDIR) $(PDIR)

vpath %.c src:lib
vpath %.o src:lib

all: $(objects) 
		$(CC) -o Complex_calc $^ -lm -ldl

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
	
