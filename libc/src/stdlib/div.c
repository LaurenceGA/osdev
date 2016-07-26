#include "stdlib.h"

div_t div(int numer, int denom) {
	div_t _div;
	_div.quot = numer / denom;
	_div.rem = numer % denom;
	return _div;
}

ldiv_t ldiv(long numer, long denom) {
	ldiv_t _ldiv;
	_ldiv.quot = numer / denom;
	_ldiv.rem = numer % denom;
	return _ldiv;
}
