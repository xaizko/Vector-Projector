#include "Vector.h"
#include <cmath>

vector3 addVector(vector3 v1, vector3 v2) {
	vector3 v3;

	v3.x_cord = v1.x_cord + v2.x_cord;
	v3.y_cord = v1.y_cord + v2.y_cord;
	v3.z_cord = v1.z_cord + v2.z_cord;

	return v3;
}

vector3 subtractVector(vector3 v1, vector3 v2) {
	vector3 v3;

	v3.x_cord = v1.x_cord - v2.x_cord;
	v3.y_cord = v1.y_cord - v2.y_cord;
	v3.z_cord = v1.z_cord - v2.z_cord;

	return v3;
}

vector3 dotProduct(vector3 v1, vector3 v2) {
	vector3 v3;
	v3.x_cord = (v1.x_cord * v2.x_cord);
	v3.y_cord = (v1.y_cord * v2.y_cord);
	v3.z_cord = (v1.z_cord * v2.z_cord);

	return v3;
}

vector3 crossProduct(vector3 v1, vector3 v2) {
	vector3 v3;
	v3.x_cord = (v1.y_cord * v2.z_cord) - (v1.y_cord * v2.z_cord);
	v3.y_cord = -((v1.x_cord * v2.z_cord) - (v1.z_cord * v2.x_cord));
	v3.z_cord = (v1.x_cord * v2.y_cord) - (v1.y_cord * v2.x_cord);

	return v3;
}

int calculateMagnitude(vector3 v) {
	return sqrt((v.x_cord*v.x_cord) + (v.y_cord * v.y_cord) + (v.z_cord * v.z_cord));
}
