#include <iostream>
#include <vector>

#define SHAPE vector<int>(3, 0)
using namespace std;

void processInput(int &FieldWidth, int &FieldHeight, int &NumShapes, vector<vector<int>> &ShapeTypes) {
	// Read the first two lines containing the number of lines, columns and shapes
	char comma;
	cin >> FieldWidth >> comma >> FieldHeight;
	cin >> NumShapes;

	// Populate the shapes matrix
	for (int i = 0; i < NumShapes; i++) {
		int width, height, price;
		cin >> width >> height >> price;

		ShapeTypes.push_back(SHAPE);
		ShapeTypes[i][0] = width;
		ShapeTypes[i][1] = height;
		ShapeTypes[i][2] = price;
	}
}

int getMaxPrice(vector<vector<vector<int>>> &FieldPermutations) {}

int main() {
	int FieldWidth, FieldHeight, NumShapes, MaxPrice = 0;
	vector<vector<int>> ShapeTypes;

	// Process the input
	processInput(FieldWidth, FieldHeight, NumShapes, ShapeTypes);

	// Print the maximum price and finish
	cout << getMaxPrice(FieldPermutations) << endl;
	return 0;
}