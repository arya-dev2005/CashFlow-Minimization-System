#ifndef CASH_FLOW_H
#define CASH_FLOW_H

#include "Models.h"

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class CashFlowSystem {
public:
    // Adds a transaction and creates participants if they do not already exist.
    bool addTransaction(const std::string& from, const std::string& to, double amount);

    // Removes the last entered transaction.
    bool undoLastTransaction();

    // Clears all participants, transactions, and undo history.
    void reset();

    // Computes each participant's net balance from transaction history.
    std::vector<double> calculateNetBalances() const;

    // Builds settlements with the greedy two-heap algorithm.
    SettlementResult minimizeTransactions() const;

    // Exports the current minimized settlement list to a text file.
    bool exportSettlements(const std::string& filePath) const;

    // Accessors used by UI and graph analysis.
    const std::vector<std::string>& getPeople() const;
    const std::vector<Transaction>& getTransactions() const;
    int transactionCount() const;
    int participantCount() const;

private:
    std::vector<std::string> people;
    std::unordered_map<std::string, int> personToId;
    std::vector<Transaction> transactions;
    std::stack<Transaction> undoStack;

    // Returns an existing participant id or creates a new one.
    int getOrCreatePerson(const std::string& name);
};

#endif
