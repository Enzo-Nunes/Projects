#include <iostream>
#include <vector>

#define max(a, b) (a > b ? a : b)
using namespace std;

vector<vector<int>> buildPlate(int &PlateWidth, int &PlateHeight) {
	int NumShapes;
	scanf("%d %d\n%d\n", &PlateWidth, &PlateHeight, &NumShapes);
	vector<vector<int>> Plate(PlateWidth + 1, vector<int>(PlateHeight + 1, 0));

	for (int i = 0; i < NumShapes; i++) {
		int Width, Height, Cost;
		scanf("%d %d %d\n", &Width, &Height, &Cost);
		if (Width <= PlateWidth && Height <= PlateHeight)
			Plate[Width][Height] = Cost;
		if (Height <= PlateWidth && Width <= PlateHeight)
			Plate[Height][Width] = Cost;
	}

	return Plate;
}

int getMaxPlateCost(int PlateWidth, int PlateHeight, vector<vector<int>> &Plate) {

	for (int i = 1; i <= PlateWidth; i++) {
		for (int j = 1; j <= PlateHeight; j++) {
			for (int k = 1; k < i; k++) {
				Plate[i][j] = max(Plate[i][j], Plate[k][j] + Plate[i - k][j]);
			}
			for (int k = 1; k < j; k++) {
				Plate[i][j] = max(Plate[i][j], Plate[i][k] + Plate[i][j - k]);
			}
		}
	}
	return Plate[PlateWidth][PlateHeight];
}

int main() {
	int PlateWidth, PlateHeight;

	vector<vector<int>> Plate = buildPlate(PlateWidth, PlateHeight);
	printf("%d\n", getMaxPlateCost(PlateWidth, PlateHeight, Plate));

	return 0;
}