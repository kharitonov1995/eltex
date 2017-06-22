#ifndef COMPLEX_H
#define COMPLEX_H
typedef struct st {
	double Rb;
	double Im;
} myComplex;

myComplex Add(myComplex, myComplex);
myComplex Sub(myComplex, myComplex);
myComplex Mul(myComplex, myComplex);
myComplex Div(myComplex, myComplex);

void inputComplex(myComplex *);
void printComplex(myComplex);
void scan(char *, void *);
int countPlugins();
char **getNamesPlugins(int*);
char *parseName(char*);
#endif
