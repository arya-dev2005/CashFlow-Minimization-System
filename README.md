# Cash Flow Minimization CLI

A polished C++17 console application that minimizes the number of transactions needed to settle debts among multiple people. It is designed as a resume/interview project that demonstrates greedy algorithms, heaps, graph traversal, clean architecture, input validation, and CLI UX.

## Key Features

- Menu-driven console interface
- Add, view, undo, reset, and export transactions
- Real-time net balance updates after each transaction
- Greedy settlement using two `priority_queue` heaps
- Step-by-step settlement explanation
- Simulation mode showing before vs after minimization
- Performance metrics: original transactions vs optimized settlements
- DFS/BFS graph traversal helpers
- Circular debt detection such as `A -> B -> C -> A`
- Independent group counting for disconnected debt groups

## Architecture

```text
main.cpp
UI.h / UI.cpp
CashFlow.h / CashFlow.cpp
Graph.h / Graph.cpp
Utils.h / Utils.cpp
Models.h
.vscode/tasks.json
```

### `main.cpp`

Application entry point. It creates `ConsoleUI` and starts the interactive menu loop.

### `UI.h` / `UI.cpp`

UI layer. Responsible for:

- Menu rendering
- Input prompts
- Structured tables
- Step-by-step settlement display
- Export command
- Simulation mode

It does not implement the heap algorithm directly.

### `CashFlow.h` / `CashFlow.cpp`

Service and data layer. Responsible for:

- Storing participants
- Storing transaction history
- Undo stack
- Net balance calculation
- Greedy minimization
- Settlement export

Important functions:

- `addTransaction(...)`
- `undoLastTransaction()`
- `calculateNetBalances()`
- `minimizeTransactions()`
- `exportSettlements(...)`

### `Graph.h` / `Graph.cpp`

Graph helper module. Responsible for:

- Building adjacency lists from transactions
- BFS traversal
- DFS traversal
- Directed cycle detection
- Independent group counting

### `Utils.h` / `Utils.cpp`

Reusable console helpers:

- Validated integer input
- Validated positive amount input
- Validated names
- Money formatting
- Separators
- ANSI color wrappers

### `Models.h`

Shared data structures:

- `Transaction`
- `Settlement`
- `SettlementStep`
- `SettlementResult`

## Greedy Logic

Each transaction is treated as a graph edge:

```text
A owes B 100
```

This means:

```text
A balance -= 100
B balance += 100
```

After all transactions:

- Negative balance means the person is a debtor.
- Positive balance means the person is a creditor.
- Zero-balance users are ignored during settlement.

The optimizer uses:

- A max-heap for creditors by receivable amount
- A max-heap for debtors by absolute owed amount

At each step:

1. Pick the largest debtor.
2. Pick the largest creditor.
3. Settle the smaller of the two amounts.
4. Push any remaining non-zero amount back into its heap.

This reduces the debt graph into a compact set of direct settlement transactions.

## Build and Run

### VS Code

1. Install MinGW-w64 or another compiler that provides `g++`.
2. Make sure `g++` is available in PATH.
3. Open this folder in VS Code.
4. Press `Ctrl+Shift+B`.
5. Run:

```powershell
.\cashflow.exe
```

### Terminal

```powershell
g++ -std=c++17 -Wall -Wextra -O2 main.cpp UI.cpp CashFlow.cpp Graph.cpp Utils.cpp -o cashflow.exe
.\cashflow.exe
```

## Sample Run

```text
========================================================================
Cash Flow Minimization
========================================================================
1.  Add Transaction
2.  View All Transactions
3.  View Net Balances
4.  Minimize Cash Flow
5.  Show Settlement Steps
6.  Simulation Mode
7.  Graph Analysis
8.  Export Final Settlements
9.  Undo Last Transaction
10. Reset System
11. Exit
------------------------------------------------------------------------
Choose an option: 1

========================================================================
Add Transaction
========================================================================
Debtor name  : A
Creditor name: B
Amount owed  : 100
Transaction added successfully.

ID    Name                  Status               Balance
------------------------------------------------------------------------
0     A                     Owes                  100.00
1     B                     Receives              100.00
```

Circular debt example:

```text
A owes B 100
B owes C 50
C owes A 30
```

Optimized result:

```text
#     Payer             Receiver                  Amount
------------------------------------------------------------------------
1     A                 B                          50.00
2     A                 C                          20.00

Transactions before optimization : 3
Transactions after optimization  : 2
Transactions reduced             : 1
```

Step-by-step mode:

```text
Step 1: choose largest debtor A (owes 70.00) and largest creditor B (receives 50.00).
        Settle 50.00. Remaining debtor liability: 20.00, creditor claim: 0.00.
Step 2: choose largest debtor A (owes 20.00) and largest creditor C (receives 20.00).
        Settle 20.00. Remaining debtor liability: 0.00, creditor claim: 0.00.
```

## Edge Cases Handled

- Zero-balance users
- Already settled systems
- Circular debts
- Single remaining participant after undo/reset behavior
- Large transaction lists
- Duplicate transactions
- Invalid names and amounts
- Self-debts are rejected

## Complexity

Let:

- `N` = number of participants
- `E` = number of original transactions
- `K` = number of non-zero-balance participants
- `T` = number of optimized settlement transactions

| Operation | Time | Space |
| --- | --- | --- |
| Net balance calculation | `O(E)` | `O(N)` |
| Heap construction | `O(K log K)` | `O(K)` |
| Greedy settlement | `O(T log K)` | `O(T)` |
| BFS / DFS / cycle detection | `O(N + E)` | `O(N + E)` |

Overall settlement complexity is:

```text
O(E + K log K + T log K)
```

The implementation remains efficient for large inputs while keeping the user experience suitable for demonstrations.