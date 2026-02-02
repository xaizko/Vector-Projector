struct vector {
    float x_cord;
    float y_cord;
    float z_cord;
};

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

float dotVector(vector v1, vector v2) {
    float sum = (v1.x_cord * v2.x_cord) +
		(v1.y_cord * v2.y_cord) +
		(v1.z_cord * v2.z_cord);

    return sum;
}

vector crossVector(vector v1, vector v2) {
    vector v3;

    

    return v3;
}
