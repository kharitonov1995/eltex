IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lncurses -lform -lpthread
CFLAGS = -O2 -Wall -pedantic -ansi
CC = gcc

objects = $(SDIR)/main.o $(LDIR)/manager.o
SUBDIRS = $(LDIR) $(SDIR)

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: $(objects)
		$(CC) -o manager $^ $(CFLAGS) $(LFLAGS) 

$(objects): prepare

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR):$(LDIR)

clean:
		cd src 		&& $(MAKE) clean
		cd lib		&& $(MAKE) clean
		rm -rf manager *.o html latex
