#pragma once
#ifndef PRINT_TESTS_H_
#define PRINT_TESTS_H_

#include <functional>
#include <stdexcept>
#include <exception>
#include "print.hpp"

namespace made {
    namespace test {
        using TestFunc = std::function<bool()>;

        namespace print {
            using namespace made::print;

            bool format_sample() {
                std::cout << "format(\"{1}+{1} = {0}\", 2, \"one\")";
                return format("{1}+{1} = {0}", 2, "one") == "one+one = 2";
            }

            bool format_extra_arguments() {
                std::cout << "format(\"{1}+{1} = {0}\", 2, \"one\", 5)";
                return format("{1}+{1} = {0}", 2, "one", 5) == "one+one = 2";
            }

            template <class... TArgs>
            bool format_uncorrect_expression(const std::string& test_string, TArgs&&... args) {
                std::cout << "formatting `" << test_string << "`, expect exception";
                try {
                    format(test_string, args...);
                }
                catch (const std::runtime_error& e) {
                    std::cout << std::endl << e.what();
                    return true;
                }
                return false;
            }

            bool format_extra_closing_bracket() {
                return format_uncorrect_expression("abc}{0}, b", "def");
            }

            bool format_unclosed_bracket() {
                return format_uncorrect_expression("abc{0}, b{1", "def", 15);
            }

            bool format_extra_argument_number() {
                return format_uncorrect_expression("abc{1}", "def");
            }

            bool format_uncorrect_placeolder() {
                return format_uncorrect_expression("abc{a1abc}", "def");
            }

            std::vector<TestFunc> GetTests() {
                return {
                    format_sample,
                    format_extra_arguments,
                    format_extra_closing_bracket,
                    format_unclosed_bracket,
                    format_extra_argument_number,
                    format_uncorrect_placeolder
                };
            }
        }
    }
}

#endif // !PRINT_TESTS_H_