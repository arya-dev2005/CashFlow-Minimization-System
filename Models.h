#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

constexpr double EPSILON = 1e-9;

struct Transaction {
    int from;
    int to;
    double amount;
};

struct Settlement {
    int from;
    int to;
    double amount;
};

struct SettlementStep {
    int debtor;
    int creditor;
    double debtorBefore;
    double creditorBefore;
    double settledAmount;
    double debtorRemaining;
    double creditorRemaining;
};

struct SettlementResult {
    std::vector<Settlement> settlements;
    std::vector<SettlementStep> steps;
};

#endif
