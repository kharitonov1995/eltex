#define _GNU_SOURCE 1
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct st {
	double Rb;
	double Im;
} myComplex;

typedef struct _plugin {
	myComplex (*funcPlug)(myComplex, myComplex);
	char *namePlugin;
	char *nameFunc;
	char *execFile;
} plugin;

myComplex Add(myComplex, myComplex);
myComplex Sub(myComplex, myComplex);
myComplex Mul(myComplex, myComplex);
myComplex Div(myComplex, myComplex);

void inputComplex(myComplex *);
void printComplex(myComplex);
void scan(char *, void *);
int countPlugins();
char **getMenu(struct dirent *, int);
char *parseName(char*);
int filter(const struct dirent *);
plugin* initPlugins(plugin*, struct dirent **, int);
#endif
