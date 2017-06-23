#include "../include/complex.h"

const char NAME_PLUGIN[] = "Sub";

myComplex sub(myComplex a, myComplex b) {
	myComplex result;

	result.Rb = a.Rb - b.Rb;
	result.Im = a.Im - b.Im;
	
	return result;
}
