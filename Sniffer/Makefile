IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lpcap
CFLAGS = -g -O2 -Wall -pedantic -ansi
CC = gcc

SUBDIRS = $(LDIR) $(SDIR)
OBJ = $(SDIR)/main.o $(LDIR)/sniffer.o

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

sniffer: $(OBJ)
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
		rm -rf sniffer *.o
