#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    // Clears bad input and discards the rest of the current input line.
    void clearInput();

    // Reads an integer menu/input value within a closed range.
    int readIntInRange(const std::string& prompt, int low, int high);

    // Reads a strictly positive money amount.
    double readPositiveAmount(const std::string& prompt);

    // Reads and validates a participant name.
    std::string readName(const std::string& prompt);

    // Formats a money amount with two decimal places.
    std::string formatMoney(double amount);

    // Prints a horizontal separator line.
    void printLine(char ch = '-', int width = 72);

    // Returns true when ANSI colors should be used in console output.
    bool colorsEnabled();

    // Wraps text in ANSI color codes when colors are enabled.
    std::string color(const std::string& text, const std::string& code);
}

#endif
