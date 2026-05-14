#include "VectorCalculator.cpp"
#include "operationEnums.cpp"

using namespace std;

class VectorController {
	public:	
		VectorController(){}

		void setV1(double x, double y, double z) {
			v1.x_cord = x;
			v1.y_cord = y;
			v1.z_cord = z;
		}

		void setV2(double x, double y, double z) {
			v2.x_cord = x;
			v2.y_cord = y;
			v2.z_cord = z;
		}

		void performOperation(Operations operation) {
			switch(operation) {
				case (Operations::ADD):
					v3 = addVector(v1, v2); 
					break;
				case (Operations::SUBTRACT):
					v3 = subtractVector(v1, v2); 
					break;
				case (Operations::DOT):
					v3 = dotProduct(v1, v2); 
					break;
				case (Operations::CROSS):
					v3 = crossProduct(v1, v2);
					break;
				default:
					break;

			}	
		}

	private:
		vector3 v1;
		vector3 v2;
		vector3 v3;
};
