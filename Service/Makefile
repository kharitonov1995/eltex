IDIR = include
LDIR = lib
SDIR = src
LFLAGS =
CFLAGS = -g -O2 -Wall -pedantic -ansi
CC = gcc

SUBDIRS = $(LDIR) $(SDIR)
OBJSERV = $(SDIR)/main.o $(LDIR)/service.o
OBJCLIENT = $(SDIR)/client.o $(LDIR)/service.o

vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: server client

server: $(OBJSERV)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 

$(OBJSERV): prepare

client: $(OBJCLIENT)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 

$(OBJCLIENT): prepare


.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR): $(LDIR)

clean:
		cd src 		&& $(MAKE) clean
		cd lib		&& $(MAKE) clean
		rm -rf server client fifoClient fifoServer *.o
