#include "UI.h"

#include "Graph.h"
#include "Utils.h"

#include <cmath>
#include <iomanip>
#include <iostream>

void ConsoleUI::run() {
    while (true) {
        showMenu();
        int choice = Utils::readIntInRange("Choose an option: ", 1, 11);

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewTransactions(); break;
            case 3: viewBalances(); break;
            case 4: minimizeCashFlow(); break;
            case 5: showSettlementSteps(); break;
            case 6: showSimulation(); break;
            case 7: showGraphAnalysis(); break;
            case 8: exportSettlements(); break;
            case 9: undoLastTransaction(); break;
            case 10: resetSystem(); break;
            case 11:
                std::cout << Utils::color("Goodbye.\n", "32");
                return;
        }
    }
}

void ConsoleUI::showMenu() const {
    printHeader("Cash Flow Minimization");
    std::cout << "1.  Add Transaction\n";
    std::cout << "2.  View All Transactions\n";
    std::cout << "3.  View Net Balances\n";
    std::cout << "4.  Minimize Cash Flow\n";
    std::cout << "5.  Show Settlement Steps\n";
    std::cout << "6.  Simulation Mode\n";
    std::cout << "7.  Graph Analysis\n";
    std::cout << "8.  Export Final Settlements\n";
    std::cout << "9.  Undo Last Transaction\n";
    std::cout << "10. Reset System\n";
    std::cout << "11. Exit\n";
    Utils::printLine();
}

void ConsoleUI::addTransaction() {
    printHeader("Add Transaction");
    std::string from = Utils::readName("Debtor name  : ");
    std::string to = Utils::readName("Creditor name: ");

    if (from == to) {
        std::cout << Utils::color("Rejected: debtor and creditor cannot be the same person.\n", "31");
        return;
    }

    double amount = Utils::readPositiveAmount("Amount owed  : ");
    if (!system.addTransaction(from, to, amount)) {
        std::cout << Utils::color("Transaction was not added.\n", "31");
        return;
    }

    std::cout << Utils::color("Transaction added successfully.\n", "32");
    printBalancesTable();
}

void ConsoleUI::viewTransactions() const {
    printHeader("Transaction History");
    const std::vector<std::string>& people = system.getPeople();
    const std::vector<Transaction>& transactions = system.getTransactions();

    if (transactions.empty()) {
        std::cout << "No transactions recorded yet.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "#"
              << std::setw(18) << "Debtor"
              << std::setw(18) << "Creditor"
              << std::right << std::setw(14) << "Amount" << '\n';
    Utils::printLine();

    for (int i = 0; i < static_cast<int>(transactions.size()); ++i) {
        const Transaction& transaction = transactions[i];
        std::cout << std::left << std::setw(6) << i + 1
                  << std::setw(18) << people[transaction.from]
                  << std::setw(18) << people[transaction.to]
                  << std::right << std::setw(14) << Utils::formatMoney(transaction.amount) << '\n';
    }
}

void ConsoleUI::viewBalances() const {
    printHeader("Net Balances");
    printBalancesTable();
}

void ConsoleUI::minimizeCashFlow() const {
    printHeader("Optimized Settlements");
    SettlementResult result = system.minimizeTransactions();
    printSettlementsTable(result);
    printPerformanceMetrics(result);
}

void ConsoleUI::showSettlementSteps() const {
    printHeader("Step-by-Step Settlement Mode");
    SettlementResult result = system.minimizeTransactions();
    const std::vector<std::string>& people = system.getPeople();

    if (result.steps.empty()) {
        std::cout << "No steps needed. Everyone is already settled.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(result.steps.size()); ++i) {
        const SettlementStep& step = result.steps[i];
        std::cout << "Step " << i + 1 << ": choose largest debtor "
                  << Utils::color(people[step.debtor], "31") << " (owes "
                  << Utils::formatMoney(step.debtorBefore) << ") and largest creditor "
                  << Utils::color(people[step.creditor], "32") << " (receives "
                  << Utils::formatMoney(step.creditorBefore) << ").\n";
        std::cout << "        Settle " << Utils::formatMoney(step.settledAmount)
                  << ". Remaining debtor liability: " << Utils::formatMoney(step.debtorRemaining)
                  << ", creditor claim: " << Utils::formatMoney(step.creditorRemaining) << ".\n";
    }
}

void ConsoleUI::showSimulation() const {
    printHeader("Simulation Mode");
    std::cout << "Before minimization:\n";
    viewTransactions();
    std::cout << '\n';

    std::cout << "After minimization:\n";
    SettlementResult result = system.minimizeTransactions();
    printSettlementsTable(result);
    printPerformanceMetrics(result);
}

void ConsoleUI::showGraphAnalysis() const {
    printHeader("Graph Analysis");
    const std::vector<std::string>& people = system.getPeople();

    if (people.empty()) {
        std::cout << "No participants available.\n";
        return;
    }

    DebtGraph graph(people, system.getTransactions());
    std::cout << "Participants          : " << system.participantCount() << '\n';
    std::cout << "Original transactions : " << system.transactionCount() << '\n';
    std::cout << "Circular debt present : " << (graph.hasCircularDebt() ? "Yes" : "No") << '\n';
    std::cout << "Independent groups    : " << graph.countIndependentGroups() << '\n';

    int start = Utils::readIntInRange("Start index for BFS/DFS (0 to "
                                      + std::to_string(system.participantCount() - 1) + "): ",
                                      0,
                                      system.participantCount() - 1);
    std::vector<int> bfsOrder = graph.bfs(start);
    std::vector<int> dfsOrder = graph.dfs(start);

    std::cout << "BFS order             : ";
    for (int id : bfsOrder) {
        std::cout << people[id] << ' ';
    }
    std::cout << "\nDFS order             : ";
    for (int id : dfsOrder) {
        std::cout << people[id] << ' ';
    }
    std::cout << '\n';
}

void ConsoleUI::exportSettlements() const {
    printHeader("Export Settlements");
    std::string filePath;
    std::cout << "Output file path [settlements.txt]: ";
    std::getline(std::cin, filePath);
    if (filePath.empty()) {
        filePath = "settlements.txt";
    }

    if (system.exportSettlements(filePath)) {
        std::cout << Utils::color("Exported settlements to " + filePath + "\n", "32");
    } else {
        std::cout << Utils::color("Could not write to " + filePath + "\n", "31");
    }
}

void ConsoleUI::undoLastTransaction() {
    printHeader("Undo Last Transaction");
    if (system.undoLastTransaction()) {
        std::cout << Utils::color("Last transaction removed.\n", "32");
        printBalancesTable();
    } else {
        std::cout << "Nothing to undo.\n";
    }
}

void ConsoleUI::resetSystem() {
    printHeader("Reset System");
    int confirm = Utils::readIntInRange("Reset all data? 1 = yes, 2 = no: ", 1, 2);
    if (confirm == 1) {
        system.reset();
        std::cout << Utils::color("System reset complete.\n", "32");
    } else {
        std::cout << "Reset cancelled.\n";
    }
}

void ConsoleUI::printHeader(const std::string& title) const {
    std::cout << '\n';
    Utils::printLine('=');
    std::cout << Utils::color(title, "36") << '\n';
    Utils::printLine('=');
}

void ConsoleUI::printBalancesTable() const {
    const std::vector<std::string>& people = system.getPeople();
    std::vector<double> balances = system.calculateNetBalances();

    if (people.empty()) {
        std::cout << "No participants available.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(22) << "Name"
              << std::setw(14) << "Status"
              << std::right << std::setw(14) << "Balance" << '\n';
    Utils::printLine();

    for (int i = 0; i < static_cast<int>(people.size()); ++i) {
        std::string status = "Settled";
        double shownAmount = 0.0;
        if (balances[i] > EPSILON) {
            status = "Receives";
            shownAmount = balances[i];
        } else if (balances[i] < -EPSILON) {
            status = "Owes";
            shownAmount = -balances[i];
        }

        std::cout << std::left << std::setw(6) << i
                  << std::setw(22) << people[i]
                  << std::setw(14) << status
                  << std::right << std::setw(14) << Utils::formatMoney(shownAmount) << '\n';
    }
}

void ConsoleUI::printSettlementsTable(const SettlementResult& result) const {
    const std::vector<std::string>& people = system.getPeople();

    if (result.settlements.empty()) {
        std::cout << "No settlement transactions needed.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "#"
              << std::setw(18) << "Payer"
              << std::setw(18) << "Receiver"
              << std::right << std::setw(14) << "Amount" << '\n';
    Utils::printLine();

    for (int i = 0; i < static_cast<int>(result.settlements.size()); ++i) {
        const Settlement& settlement = result.settlements[i];
        std::cout << std::left << std::setw(6) << i + 1
                  << std::setw(18) << people[settlement.from]
                  << std::setw(18) << people[settlement.to]
                  << std::right << std::setw(14) << Utils::formatMoney(settlement.amount) << '\n';
    }
}

void ConsoleUI::printPerformanceMetrics(const SettlementResult& result) const {
    std::cout << '\n';
    Utils::printLine();
    std::cout << "Transactions before optimization : " << system.transactionCount() << '\n';
    std::cout << "Transactions after optimization  : " << result.settlements.size() << '\n';
    std::cout << "Transactions reduced             : "
              << system.transactionCount() - static_cast<int>(result.settlements.size()) << '\n';
}
