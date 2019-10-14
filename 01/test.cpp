#include <vector>
#include <iostream>
#include "calc.h"

namespace CalculatorTests {

    struct TestCase {
        const char* expression;
        bool valid;
        int answer;
    };

    std::vector<TestCase> getTests() {
        std::vector<TestCase> result = {
            { "42", true, 42 },
            { "45 * 23 - 10 / 5", true, 1033 },
            { "4/5/", false, 0 },
            { "88/8/11*   10 + 100 + 4", true, 114 },
            { "1 + 123456789", true, 123456790 },
            { "1 + 1234567890", false, 0 },
            { "1 --1--2--3+-4", true, 3 },
            { "25 - 8 / 6 - 0 / 0", false, 0 },
            { "25 - 8 / 6 - 0 */ 0", false, 0 }
        };
        return result;
    }

    bool runTest(TestCase test) {
        try {
            if (!test.valid) {
                int result;
                try {
                    result = Calculator::calc(test.expression);
                }
                catch (...) {
                    return true;
                }
                std::cerr << "Failed" << std::endl;
                std::cerr << "Expected expression '" << test.expression << "' to fail, got " << result << " instead" << std::endl;
                return false;
            }
            int result = Calculator::calc(test.expression);
            if (result == test.answer) {
                return true;
            }
            std::cerr << "Failed" << std::endl;
            std::cerr << "Expected expression '" << test.expression << "' to be " << test.answer << ", got " << result << " instead" << std::endl;
        }
        catch (...) {
            std::cerr << "Failed" << std::endl;
            std::cerr << "Expected expression '" << test.expression << "' to be " << test.answer << ", got error instead" << std::endl;
            return false;
        }
        return false;
    }

    int runTests() {
        std::vector<TestCase> tests = getTests();
        std::size_t testsCount = tests.size();
        bool failed = false;
        for (std::size_t i = 0; i < testsCount; ++i) {
            std::cout << "Running test " << i + 1 << "/" << testsCount << "... ";
            bool testResult = runTest(tests[i]);
            failed = failed || !testResult;
            if (testResult) {
                std::cout << "OK";
            }
            std::cout << std::endl;
        }
        if (!failed) {
            std::cout << "All tests passed" << std::endl;
        }
        return 0;
    }
}

int main() {
    CalculatorTests::runTests();
}