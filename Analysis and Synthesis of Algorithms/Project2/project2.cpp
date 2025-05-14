#include <iostream>
#include <stack>
#include <vector>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
using namespace std;

void buildGraph(vector<vector<int>> &Graph, int &Nodes, int &Edges) {

	scanf("%d %d\n", &Nodes, &Edges);
	Graph = vector<vector<int>>(Nodes);

	for (int i = 0; i < Edges; i++) {
		int From, To;
		scanf("%d %d\n", &From, &To);
		Graph[From - 1].push_back(To - 1);
	}
}

// Applies Tarjan's algorithm
void tarjanAlgorithmUtil(vector<vector<int>> &Graph, int Node, vector<int> &LowLink, vector<int> &Index, stack<int> &StackTarjan,
						 vector<bool> &OnStackTarjan, vector<vector<int>> &SCC) {
	static int CurrentIndex = 0;
	stack<pair<int, int>> StackDFS;
	StackDFS.push({Node, 0});

	// Iterative DFS
	while (!StackDFS.empty()) {
		int CurrentNode = StackDFS.top().first;
		int CurrentNeighborIndex = StackDFS.top().second;
		StackDFS.pop();

		// First time visiting this node: set the index and lowlink and push it to the stack
		if (CurrentNeighborIndex == 0) {
			Index[CurrentNode] = CurrentIndex;
			LowLink[CurrentNode] = CurrentIndex;
			CurrentIndex++;
			StackTarjan.push(CurrentNode);
			OnStackTarjan[CurrentNode] = true;
		}

		bool Backtracked = false;
		// Iterate through the neighbors of the current node
		for (size_t i = CurrentNeighborIndex; i < Graph[CurrentNode].size(); i++) {
			int Neighbor = Graph[CurrentNode][i];
			// If the neighbor hasn't been visited yet, push it to the stack and backtrack
			if (Index[Neighbor] == -1) {
				StackDFS.push({CurrentNode, i + 1});
				StackDFS.push({Neighbor, 0});
				Backtracked = true;
				break;
				// If the neighbor is on the stack, update the lowlink of the current node
			} else if (OnStackTarjan[Neighbor]) {
				LowLink[CurrentNode] = min(LowLink[CurrentNode], Index[Neighbor]);
			}
		}

		// If we didn't backtrack, we're done with this node
		if (!Backtracked) {
			// If the current node is a root node, pop the stack and generate a new SCC
			if (LowLink[CurrentNode] == Index[CurrentNode]) {
				vector<int> Component;
				int Top;
				do {
					Top = StackTarjan.top();
					StackTarjan.pop();
					OnStackTarjan[Top] = false;
					Component.push_back(Top);
				} while (Top != CurrentNode);
				SCC.push_back(Component);
			}

			// Update the lowlink of the parent node
			if (!StackDFS.empty()) {
				int Parent = StackDFS.top().first;
				LowLink[Parent] = min(LowLink[Parent], LowLink[CurrentNode]);
			}
		}
	}
}

// Applies Tarjan's algorithm to every node in the graph
void tarjanAlgorithm(vector<vector<int>> &Graph, int NumNodes, vector<vector<int>> &SCC) {
	vector<int> LowLink(NumNodes, -1);
	vector<int> Index(NumNodes, -1);
	vector<bool> OnStackTarjan(NumNodes, false);
	stack<int> StackTarjan;

	// Apply Tarjan's algorithm to each node. Cycle required by non-fully connected graphs.
	for (int i = 0; i < NumNodes; i++) {
		if (Index[i] == -1) {
			tarjanAlgorithmUtil(Graph, i, LowLink, Index, StackTarjan, OnStackTarjan, SCC);
		}
	}
}

// Builds a DAG from a graph and its SCCs
void buildDAG(vector<vector<int>> &Graph, int &Nodes, vector<vector<int>> &SCC) {

	// Create a map from node to SCC index
	vector<int> nodeToSCC(Nodes, -1);
	Nodes = (int)SCC.size();

	for (int i = 0; i < Nodes; i++) {
		for (int node : SCC[i]) {
			nodeToSCC[node] = i;
		}
	}

	vector<vector<int>> DAG(Nodes);
	for (int i = 0; i < Nodes; i++) {
		for (int node : SCC[i]) {
			for (int neighbor : Graph[node]) {
				int neighborSCC = nodeToSCC[neighbor];
				if (i != neighborSCC) {
					DAG[i].push_back(neighborSCC);
				}
			}
		}
	}

	Graph = DAG;
}

// Receives a topologically, decreasingly sorted DAG and returns the longest path
int findLongestPathLength(vector<vector<int>> &Graph, int Nodes) {
	vector<int> visited(Nodes, 0);
	int maxPath = 0;

	for (int i = Nodes - 1; i >= 0; i--) {
		for (int j : Graph[i]) {
			visited[j] = max(visited[j], visited[i] + 1);
			maxPath = max(maxPath, visited[j]);
		}
	}

	return maxPath;
}

int main() {
	int Nodes, Edges;
	vector<vector<int>> Graph, SCC;

	buildGraph(Graph, Nodes, Edges);
	tarjanAlgorithm(Graph, Nodes, SCC);
	buildDAG(Graph, Nodes, SCC);
	printf("%d\n", findLongestPathLength(Graph, Nodes));

	return 0;
}