#include "../include/complex.h"

const char NAME_PLUGIN[] = "Add";

myComplex add(myComplex a, myComplex b) {
	myComplex result;

	result.Rb = a.Rb + b.Rb;
	result.Im = a.Im + b.Im;
	
	return result;
}
