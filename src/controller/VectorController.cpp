#include "VectorCalculator.cpp"
#include "operationEnums.cpp"

using namespace std;

class VectorController {
	public:	
		VectorController(){}

		void setV1(double x, double y, double z) {
			v1.x = x;
			v1.y = y;
			v1.z = z;
		}

		void setV2(double x, double y, double z) {
			v2.x = x;
			v2.y = y;
			v2.z = z;
		}

		Vector3 getResult() {
			return v3;
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
		Vector3 v1;
		Vector3 v2;
		Vector3 v3;
};
