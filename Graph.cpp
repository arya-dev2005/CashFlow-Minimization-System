#include "Graph.h"

#include <queue>

DebtGraph::DebtGraph(const std::vector<std::string>& people,
                     const std::vector<Transaction>& transactions)
    : adjacency(people.size()), undirected(people.size()) {
    for (const Transaction& transaction : transactions) {
        adjacency[transaction.from].push_back({transaction.to, transaction.amount});
        undirected[transaction.from].push_back(transaction.to);
        undirected[transaction.to].push_back(transaction.from);
    }
}

std::vector<int> DebtGraph::bfs(int start) const {
    std::vector<int> order;
    if (start < 0 || start >= static_cast<int>(adjacency.size())) {
        return order;
    }

    std::vector<bool> visited(adjacency.size(), false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);

        for (const auto& edge : adjacency[node]) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    return order;
}

std::vector<int> DebtGraph::dfs(int start) const {
    std::vector<int> order;
    if (start < 0 || start >= static_cast<int>(adjacency.size())) {
        return order;
    }

    std::vector<bool> visited(adjacency.size(), false);
    dfsVisit(start, visited, order);
    return order;
}

bool DebtGraph::hasCircularDebt() const {
    std::vector<int> state(adjacency.size(), 0);
    for (int node = 0; node < static_cast<int>(adjacency.size()); ++node) {
        if (state[node] == 0 && hasCycleFrom(node, state)) {
            return true;
        }
    }
    return false;
}

int DebtGraph::countIndependentGroups() const {
    int groups = 0;
    std::vector<bool> visited(undirected.size(), false);

    for (int start = 0; start < static_cast<int>(undirected.size()); ++start) {
        if (visited[start]) {
            continue;
        }

        ++groups;
        std::queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbor : undirected[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    return groups;
}

void DebtGraph::dfsVisit(int node, std::vector<bool>& visited, std::vector<int>& order) const {
    visited[node] = true;
    order.push_back(node);

    for (const auto& edge : adjacency[node]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            dfsVisit(neighbor, visited, order);
        }
    }
}

bool DebtGraph::hasCycleFrom(int node, std::vector<int>& state) const {
    state[node] = 1;
    for (const auto& edge : adjacency[node]) {
        int neighbor = edge.first;
        if (state[neighbor] == 1) {
            return true;
        }
        if (state[neighbor] == 0 && hasCycleFrom(neighbor, state)) {
            return true;
        }
    }

    state[node] = 2;
    return false;
}
