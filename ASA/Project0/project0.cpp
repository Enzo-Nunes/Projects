#include <iostream>
#include <vector>
using namespace std;

int main() {
	// Declare variables to store input
	int N, M;

	// Read the first line containing N and M
	char comma;
	cin >> N >> comma >> M;

	// Create a vector of vectors to store the graph
	vector<int> graphOutgoing(N + 1);
	vector<int> graphIncoming(N + 1);

	// Input Matrix
	vector<vector<int>> inputMatrix(N, vector<int>(N, 0));

	// Common frienships matrix
	vector<vector<int>> friendshipMatrix(N, vector<int>(N, 0));

	// Populate the edge vectors and the input matrix
	for (int i = 0; i < M; i++) {
		// Read the next line containing the vertices of the edge
		int u, v;
		cin >> u >> v;

		// Increment the number of outgoing and incoming edges
		graphOutgoing[u]++;
		graphIncoming[v]++;

		inputMatrix[i][0] = u;
		inputMatrix[i][1] = v;
	}

	// First Histogram
	cout << "Histograma 1" << endl;
	for (int i = 0; i < N; i++) {
		int count = 0;
		for (int j = 1; j <= (int)graphOutgoing.size(); j++) {
			if (graphOutgoing[j] == i) {
				count++;
			}
		}
		cout << count << endl;
	}

	// Second Histogram
	cout << "Histograma 2" << endl;
	for (int i = 0; i < N; i++) {
		int count = 0;
		for (int j = 1; j <= (int)graphIncoming.size(); j++) {
			if (graphIncoming[j] == i) {
				count++;
			}
		}
		cout << count << endl;
	}

	// Friendship Matrix
	for (int i = 0; i < M; i++) {
		int current = inputMatrix[i][0];
		int toCheck = inputMatrix[i][1];
		for (int j = 0; j < M; j++) {
			if (inputMatrix[j][1] == toCheck) {
				friendshipMatrix[current - 1][inputMatrix[j][0] - 1]++;
			}
		}
	}

	// Print Friendship Matrix
	cout << endl;
	for (int i = 0; i < N; i++) {
		cout << friendshipMatrix[i][0];
		for (int j = 1; j < N; j++) {
			cout << " " << friendshipMatrix[i][j];
		}
		cout << endl;
	}

	return 0;
}
