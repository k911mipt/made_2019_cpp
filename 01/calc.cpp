#include <math.h>
#include <iostream>
#include <cstring>
#include <string>
#include "calc.h"

#ifndef INT_MAX
    #define INT_MAX INT32_MAX
#endif

namespace Calculator {
    const int maxNumberLength = static_cast<int>(log10(INT_MAX));
    const char space = ' ';
    const char minus = '-';

    inline void skipSpaces(const char* &expression) {
        while (*expression == space) { 
            expression++; 
        }
    }

    inline void skipSign(const char* &expression) {
        if (*expression == minus) {
            expression++;
        }
    }

    int parseNumber(const char* &expression) {
        skipSpaces(expression);
        const char* start = expression;
        skipSign(expression);
        int len = 0;
        while ((*expression >= '0') && (*expression <= '9') && (len < INT_MAX)) {
            expression++;
            len++;
        }
        if (len == 0) {
            std::string message("Expected digit '0-9', got '");
            message.append(1, (*start));
            message.append("'");
            throw std::invalid_argument(message);
        }
        if (len > maxNumberLength) {
            std::string message = "Can't process numbers with more, than " + std::to_string(maxNumberLength) + " digits";
            throw std::invalid_argument(message);
        }
        skipSpaces(expression);
        return atoi(start);
    }

    int parseMultDiv(const char* &expression) {
        int result = parseNumber(expression);
        while ((*expression == '/') || (*expression == '*')) {
            if (*expression == '/') {
                expression++;
                int denominator = parseNumber(expression);
                if (denominator == 0) {
                    throw std::invalid_argument("Division by zero. Result is not determined");
                }
                result /= denominator;
            }
            else if (*expression == '*') {
                expression++;
                int multiplier = parseNumber(expression);
                result *= multiplier;
            }
        }
        return result;
    }

    int parseAddSub(const char* &expression) {
        int result = parseMultDiv(expression);
        while ((*expression == '+') || (*expression == '-')) {
            if (*expression == '+') {
                expression++;
                int term = parseMultDiv(expression);
                result += term;
            }
            else if (*expression == '-') {
                expression++;
                int term = parseMultDiv(expression);
                result -= term;
            }
        }
        return result;
    }

    int calc(const char* expression) {
        return parseAddSub(expression);
    }

}
