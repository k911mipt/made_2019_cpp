#pragma once
#ifndef PRINT_TESTS_H_
#define PRINT_TESTS_H_

#include <functional>
#include <iostream>

#include "print.hpp"

namespace made {

    namespace test {

        using TestFunc = std::function<bool()>;
        using TestGetter = std::function<std::vector<TestFunc>()>;

        namespace print {

            template <class... TArgs>
            bool format_uncorrect_expression(const std::string& test_string, TArgs&&... args) {
                std::cout << "formatting `" << test_string << "`, expect exception";
                try {
                    made::print::format(test_string, args...);
                }
                catch (const std::runtime_error& e) {
                    std::cout << std::endl << e.what();
                    return true;
                }
                return false;
            }

            std::vector<TestFunc> GetTests();
        }
    }
}

#endif // !PRINT_TESTS_H_