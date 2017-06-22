#include "../include/complex.h"

const char NAME_FUNC[] = "div";

myComplex div(myComplex a, myComplex b) {
	myComplex result;
	
	result.Rb = ((a.Rb * b.Rb) + (a.Im * b.Im)) / (pow(b.Rb, 2) + pow(b.Im, 2));
	result.Im = ((b.Rb * a.Im) - (a.Rb * b.Im)) / (pow(b.Rb, 2) + pow(b.Im, 2));
	
	return result;
}

