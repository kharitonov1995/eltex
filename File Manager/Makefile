IDIR = include
LDIR = lib
SDIR = src
LIBS = -lncurses -lmenu

CC = gcc

objects = main.o manager.o
SUBDIRS = $(LDIR) $(SDIR)

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR):$(LDIR)

all: $(objects)
		$(CC) -o manager $< $(LIBS)
