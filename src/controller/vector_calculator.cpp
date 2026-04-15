#include "vector.h"
#include <cmath>

vector addVector(vector v1, vector v2) {
	vector v3;

	v3.x_cord = v1.x_cord + v2.x_cord;
	v3.y_cord = v1.y_cord + v2.y_cord;
	v3.z_cord = v1.z_cord + v2.z_cord;

	return v3;
}

vector subtractVector(vector v1, vector v2) {
	vector v3;

	v3.x_cord = v1.x_cord - v2.x_cord;
	v3.y_cord = v1.y_cord - v2.y_cord;
	v3.z_cord = v1.z_cord - v2.z_cord;

	return v3;
}

float dotProduct(vector v1, vector v2) {
	float sum = (v1.x_cord * v2.x_cord) +
		(v1.y_cord * v2.y_cord) +
		(v1.z_cord * v2.z_cord);

	return sum;
}

vector crossProduct(vector v1, vector v2) {
	vector v3;
	v3.x_cord = (v1.y_cord * v2.z_cord) - (v1.y_cord * v2.z_cord);
	v3.y_cord = -((v1.x_cord * v2.z_cord) - (v1.z_cord * v2.x_cord));
	v3.z_cord = (v1.x_cord * v2.y_cord) - (v1.y_cord * v2.x_cord);

	return v3;
}

int calculateMagnitude(vector v) {
	return sqrt((v.x_cord*v.x_cord) + (v.y_cord * v.y_cord) + (v.z_cord * v.z_cord));
}
