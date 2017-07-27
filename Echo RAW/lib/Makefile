CFLAGS = -g -O2 -Wall -pedantic -ansi 
LFLAGS = 

vpath %.h ../include/
		
echo.o: echo.c echo.h
		gcc -c $< $(CFLAGS) $(LFLAGS) 

clean:
		rm -rf *.o
