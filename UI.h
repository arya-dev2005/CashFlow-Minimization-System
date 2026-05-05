#ifndef UI_H
#define UI_H

#include "CashFlow.h"

class ConsoleUI {
public:
    // Starts the interactive menu loop.
    void run();

private:
    CashFlowSystem system;

    // Menu actions.
    void showMenu() const;
    void addTransaction();
    void viewTransactions() const;
    void viewBalances() const;
    void minimizeCashFlow() const;
    void showSettlementSteps() const;
    void showSimulation() const;
    void showGraphAnalysis() const;
    void exportSettlements() const;
    void undoLastTransaction();
    void resetSystem();

    // Shared rendering helpers.
    void printHeader(const std::string& title) const;
    void printBalancesTable() const;
    void printSettlementsTable(const SettlementResult& result) const;
    void printPerformanceMetrics(const SettlementResult& result) const;
};

#endif
