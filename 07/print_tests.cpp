#include "print_tests.hpp"

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
