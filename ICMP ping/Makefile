IDIR = include
LDIR = lib
SDIR = src
LFLAGS = 
CFLAGS = -g -O2 -Wall -pedantic -ansi
CC = gcc

SUBDIRS = $(LDIR) $(SDIR)
OBJ = $(SDIR)/main.o $(LDIR)/icmp.o

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

mping: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 

$(OBJ): prepare

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR): $(LDIR)

clean:
		cd src && $(MAKE) clean
		cd lib && $(MAKE) clean
		rm -rf mping *.o
