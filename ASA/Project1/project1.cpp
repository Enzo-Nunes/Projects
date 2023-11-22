#include <iostream>
#include <vector>

#define max(a, b) (a > b ? a : b)
using namespace std;

typedef struct ShapeType {
	int width, height, price;
} ShapeType;

void processInput(int &FieldWidth, int &FieldHeight, int &NumShapes, vector<ShapeType> &ShapeTypes) {
	// Read the first two lines containing the number of lines, columns and shapes
	cin >> FieldWidth >> FieldHeight >> NumShapes;

	// Populate the shapes matrix
	for (int i = 0; i < NumShapes; i++) {
		int width, height, price;
		cin >> width >> height >> price;

		ShapeType newShape;
		newShape.width	= width;
		newShape.height	= height;
		newShape.price	= price;
		ShapeTypes.push_back(newShape);
	}
}

void addRotations(int &NumShapes, vector<ShapeType> &ShapeTypes) {
	// Add the rotated shapes to the shapes matrix
	for (int i = 0; i < NumShapes; i++) {
		ShapeType newShape;
		newShape.width	= ShapeTypes[i].height;
		newShape.height	= ShapeTypes[i].width;
		newShape.price	= ShapeTypes[i].price;
		ShapeTypes.push_back(newShape);
	}
	NumShapes *= 2;
}

// Dynamic programming solution
int getMaxFieldPrice(int FieldWidth, int FieldHeight, int NumShapes, vector<ShapeType> &ShapeTypes) {}

int main() {
	int FieldWidth, FieldHeight, NumShapes;
	vector<ShapeType> ShapeTypes;

	// Process the input
	processInput(FieldWidth, FieldHeight, NumShapes, ShapeTypes);
	addRotations(NumShapes, ShapeTypes);

	// Print the maximum price and finish
	cout << getMaxFieldPrice(FieldWidth, FieldHeight, NumShapes, ShapeTypes) << endl;
	return 0;
}