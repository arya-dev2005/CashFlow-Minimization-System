#include "CashFlow.h"

#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <queue>

namespace {
    struct HeapNode {
        double amount;
        int person;

        bool operator<(const HeapNode& other) const {
            if (std::fabs(amount - other.amount) > EPSILON) {
                return amount < other.amount;
            }
            return person > other.person;
        }
    };

    // Pushes positive balances into creditors and negative balances into debtors.
    void buildHeaps(const std::vector<double>& balances,
                    std::priority_queue<HeapNode>& creditors,
                    std::priority_queue<HeapNode>& debtors) {
        for (int person = 0; person < static_cast<int>(balances.size()); ++person) {
            if (balances[person] > EPSILON) {
                creditors.push({balances[person], person});
            } else if (balances[person] < -EPSILON) {
                debtors.push({-balances[person], person});
            }
        }
    }
}

int CashFlowSystem::getOrCreatePerson(const std::string& name) {
    auto it = personToId.find(name);
    if (it != personToId.end()) {
        return it->second;
    }

    int id = static_cast<int>(people.size());
    people.push_back(name);
    personToId[name] = id;
    return id;
}

bool CashFlowSystem::addTransaction(const std::string& from, const std::string& to, double amount) {
    if (from == to || amount <= EPSILON) {
        return false;
    }

    Transaction transaction{getOrCreatePerson(from), getOrCreatePerson(to), amount};
    transactions.push_back(transaction);
    undoStack.push(transaction);
    return true;
}

bool CashFlowSystem::undoLastTransaction() {
    if (undoStack.empty() || transactions.empty()) {
        return false;
    }

    undoStack.pop();
    transactions.pop_back();
    return true;
}

void CashFlowSystem::reset() {
    people.clear();
    personToId.clear();
    transactions.clear();
    while (!undoStack.empty()) {
        undoStack.pop();
    }
}

std::vector<double> CashFlowSystem::calculateNetBalances() const {
    std::vector<double> balances(people.size(), 0.0);
    for (const Transaction& transaction : transactions) {
        balances[transaction.from] -= transaction.amount;
        balances[transaction.to] += transaction.amount;
    }
    return balances;
}

SettlementResult CashFlowSystem::minimizeTransactions() const {
    std::priority_queue<HeapNode> creditors;
    std::priority_queue<HeapNode> debtors;
    buildHeaps(calculateNetBalances(), creditors, debtors);

    SettlementResult result;
    while (!creditors.empty() && !debtors.empty()) {
        HeapNode creditor = creditors.top();
        HeapNode debtor = debtors.top();
        creditors.pop();
        debtors.pop();

        double settledAmount = std::min(creditor.amount, debtor.amount);
        double creditorRemaining = creditor.amount - settledAmount;
        double debtorRemaining = debtor.amount - settledAmount;

        result.settlements.push_back({debtor.person, creditor.person, settledAmount});
        result.steps.push_back({
            debtor.person,
            creditor.person,
            debtor.amount,
            creditor.amount,
            settledAmount,
            debtorRemaining,
            creditorRemaining
        });

        if (creditorRemaining > EPSILON) {
            creditors.push({creditorRemaining, creditor.person});
        }
        if (debtorRemaining > EPSILON) {
            debtors.push({debtorRemaining, debtor.person});
        }
    }

    return result;
}

bool CashFlowSystem::exportSettlements(const std::string& filePath) const {
    std::ofstream out(filePath);
    if (!out) {
        return false;
    }

    SettlementResult result = minimizeTransactions();
    out << "Cash Flow Minimization - Final Settlements\n";
    out << "Participants: " << people.size() << "\n";
    out << "Original transactions: " << transactions.size() << "\n";
    out << "Optimized transactions: " << result.settlements.size() << "\n\n";

    if (result.settlements.empty()) {
        out << "No settlements needed. Everyone is already balanced.\n";
        return true;
    }

    for (const Settlement& settlement : result.settlements) {
        out << people[settlement.from] << " pays " << people[settlement.to]
            << " : " << Utils::formatMoney(settlement.amount) << "\n";
    }

    return true;
}

const std::vector<std::string>& CashFlowSystem::getPeople() const {
    return people;
}

const std::vector<Transaction>& CashFlowSystem::getTransactions() const {
    return transactions;
}

int CashFlowSystem::transactionCount() const {
    return static_cast<int>(transactions.size());
}

int CashFlowSystem::participantCount() const {
    return static_cast<int>(people.size());
}
