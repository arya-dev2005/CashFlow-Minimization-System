#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace {
    bool isValidName(const std::string& name) {
        if (name.empty()) {
            return false;
        }

        return std::all_of(name.begin(), name.end(), [](unsigned char ch) {
            return std::isalnum(ch) || ch == '_' || ch == '-';
        });
    }
}

namespace Utils {
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readIntInRange(const std::string& prompt, int low, int high) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= low && value <= high) {
                clearInput();
                return value;
            }

            std::cout << color("Invalid choice. Enter a number from " + std::to_string(low)
                               + " to " + std::to_string(high) + ".\n", "31");
            clearInput();
        }
    }

    double readPositiveAmount(const std::string& prompt) {
        double value = 0.0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value > 0.0) {
                clearInput();
                return value;
            }

            std::cout << color("Invalid amount. Enter a positive number.\n", "31");
            clearInput();
        }
    }

    std::string readName(const std::string& prompt) {
        std::string name;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, name);

            if (isValidName(name)) {
                return name;
            }

            std::cout << color("Invalid name. Use letters, digits, '_' or '-' only.\n", "31");
        }
    }

    std::string formatMoney(double amount) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << amount;
        return out.str();
    }

    void printLine(char ch, int width) {
        std::cout << std::string(width, ch) << '\n';
    }

    bool colorsEnabled() {
#ifdef _WIN32
        return _isatty(_fileno(stdout)) != 0;
#else
        return isatty(fileno(stdout)) != 0;
#endif
    }

    std::string color(const std::string& text, const std::string& code) {
        if (!colorsEnabled()) {
            return text;
        }
        return "\033[" + code + "m" + text + "\033[0m";
    }
}
