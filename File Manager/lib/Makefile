LIBS = -lncurses

vpath %.h ../include/

list.o: list.c list.h
		gcc -c $< 
		
manager.o: manager.c manager.h
		gcc -c $< $(LIBS)
