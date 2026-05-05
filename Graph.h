#ifndef GRAPH_H
#define GRAPH_H

#include "Models.h"

#include <string>
#include <utility>
#include <vector>

class DebtGraph {
public:
    // Builds an adjacency list from the current people and transactions.
    DebtGraph(const std::vector<std::string>& people, const std::vector<Transaction>& transactions);

    // Returns BFS visit order from a participant.
    std::vector<int> bfs(int start) const;

    // Returns DFS visit order from a participant.
    std::vector<int> dfs(int start) const;

    // Detects whether the directed graph contains any cycle.
    bool hasCircularDebt() const;

    // Counts weakly connected independent groups.
    int countIndependentGroups() const;

private:
    std::vector<std::vector<std::pair<int, double>>> adjacency;
    std::vector<std::vector<int>> undirected;

    // Recursive DFS worker for traversal output.
    void dfsVisit(int node, std::vector<bool>& visited, std::vector<int>& order) const;

    // Recursive DFS worker for directed cycle detection.
    bool hasCycleFrom(int node, std::vector<int>& state) const;
};

#endif
