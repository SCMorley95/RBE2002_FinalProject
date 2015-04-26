#include <directionalArray.h>
#include <Arduino.h>
directionalArray::directionalArray(int x_init, int y_init) {
	_array[0] = x_init;
	_array[1] = y_init;
}
void directionalArray::rotate(int deg) {
	int xval_temp = array[0];
	int yval_temp = array[1];
	_array[0] = xval_temp*intCos(deg) - yval_temp*intSin(deg);
	_array[1] = xval_temp*intSin(deg) + yval_temp*intCos(deg);
}
int intCos(int deg) {
	if(deg == 0) {
		return 1;
	}
	else if(deg == 90) {
		return 0;
	}
	else if(deg == 180) {
		return -1;
	}
	else if(deg == 270) {
		return 0;
	}
}
int intSin(int deg) {
	if(deg == 0) {
		return 0;
	}
	else if(deg == 90) {
		return 1;
	}
	else if(deg == 180) {
		return 0;
	}
	else if(deg == 270) {
		return -1;
	}
}