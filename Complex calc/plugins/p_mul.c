#include "../include/complex.h"

const char NAME_FUNC[] = "mul";


myComplex mul(myComplex a, myComplex b) {
	myComplex result;

	result.Rb = (a.Rb * b.Rb) - (a.Im * b.Im);
	result.Im = (a.Rb * b.Im) + (b.Rb * a.Im);
	
	return result;
}
