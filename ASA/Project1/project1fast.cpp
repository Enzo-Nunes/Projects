#include <iostream>
#include <vector>

#define max(a, b) (a > b ? a : b)
using namespace std;

void printPlate(vector<vector<int>> &Plate, int PlateWidth, int PlateHeight) {
	for (int i = 1; i <= PlateHeight; i++) {
		for (int j = 1; j <= PlateWidth; j++) {
			printf("|%3d|", Plate[j][i]);
		}
		printf("\n");
	}
}

void printProgress(int i, int PlateWidth) {
	printf("\r%7.3f%%", ((float)i / PlateWidth * 100));
	fflush(stdout);
}

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
	int i, j;
	bool IsVertical = PlateWidth < PlateHeight;

	for (i = 1; i <= PlateWidth; i++) {
		for (
			IsVertical ? j = i : j = 1;
			IsVertical ? j <= PlateHeight : j <= i && j <= PlateHeight;
			j++
		) {
			for (int k = 1; k < i || k < j; k++) {
				if (k < i)
					Plate[i][j] = max(Plate[i][j], Plate[k][j] + Plate[i - k][j]);
				if (k < j)
					Plate[i][j] = max(Plate[i][j], Plate[i][k] + Plate[i][j - k]);
			}

			if (IsVertical ? j <= PlateWidth : i <= PlateHeight)
				Plate[j][i] = Plate[i][j];	
		}
		printProgress(i, PlateWidth);
	}
	printf("\n");
	return Plate[PlateWidth][PlateHeight];
}

int main() {
	int PlateWidth, PlateHeight;

	vector<vector<int>> Plate = buildPlate(PlateWidth, PlateHeight);
	printf("%d\n", getMaxPlateCost(PlateWidth, PlateHeight, Plate));
	printPlate(Plate, PlateWidth, PlateHeight);

	return 0;
}