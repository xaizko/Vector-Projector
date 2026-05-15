#include <raylib.h>
#include <cmath>

Vector3 addVector(Vector3 v1, Vector3 v2) {
	Vector3 v3;

	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;

	return v3;
}

Vector3 subtractVector(Vector3 v1, Vector3 v2) {
	Vector3 v3;

	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;

	return v3;
}

Vector3 dotProduct(Vector3 v1, Vector3 v2) {
	Vector3 v3;
	v3.x = (v1.x * v2.x);
	v3.y = (v1.y * v2.y);
	v3.z = (v1.z * v2.z);

	return v3;
}

Vector3 crossProduct(Vector3 v1, Vector3 v2) {
	Vector3 v3;
	v3.x = (v1.y * v2.z) - (v1.y * v2.z);
	v3.y = -((v1.x * v2.z) - (v1.z * v2.x));
	v3.z = (v1.x * v2.y) - (v1.y * v2.x);

	return v3;
}

int calculateMagnitude(Vector3 v) {
	return sqrt((v.x*v.x) + (v.y * v.y) + (v.z * v.z));
}
