#include <iostream>
#include <unordered_set>
#include <unordered_map>
using namespace std;

void Dfs(const unordered_map<char, unordered_set<char>>& graph, char start,
	     unordered_set<char>& visited) {
	visited.insert(start);
	cout << start << " ";
	for (auto&& vert : graph.at(start)) {
		if (visited.find(vert) == visited.end()) {
			Dfs(graph, vert, visited);
		}
	}
}

int main() {
	unordered_map<char, unordered_set<char>> graph {
		{ 'A', {{'B', 'C'}} },
		{ 'B', {{'A', 'D', 'E'}} },
		{ 'C', {{'A', 'F'}} },
		{ 'D', {{'B'}} },
		{ 'E', {{'B', 'F'}} },
		{ 'F', {{'C', 'E'}} }
	};

	unordered_set<char> visited;
	Dfs(graph, 'C', visited);

	return 0;
}
