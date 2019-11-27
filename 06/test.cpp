#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>

#include "long_arithmetic.hpp"


namespace made {

    namespace test {
        typedef bool(*TestFunc)();
        typedef std::vector<TestFunc>(*TestGetter)();

        namespace long_arithmetic {
            using namespace made::long_arithmetic;

            template <typename T1, typename T2>
            bool compare_couts(const T1& a, const T2& b) {
                std::cout << "\t" << a << "\t";
                std::stringstream stream1, stream2;
                stream1 << a;
                stream2 << b;
                return stream1.str() == stream2.str();
            }

            bool create_bigint() {
                std::cout << "creating BigInt";
                BigInt number;
                return true;
            }

            bool create_bigint_init_literal() {
                std::cout << "creating BigInt from 123456789";
                BigInt number(123456789);
                return true;
            }

            bool cout_bigint() {
                std::cout << "cout BigInt of 123456789123456789";
                auto number = 123456789123456789;
                BigInt big_num(number);
                return compare_couts(big_num, number);
            }

            bool assign_int_to_bigint() {
                std::cout << "assigning int 156 to bigint";
                BigInt a = 1;
                return compare_couts(a, 1);
                return true;
            }

            bool assign_bigint_to_bigint() {
                std::cout << "assigning bigint to bigint";
                BigInt a = 1;
                BigInt b = a;
                return compare_couts(a, 1);
                return compare_couts(a, b);
                return true;
            }

            bool adding_bigints() {
                std::cout << "adding bigints ";
                BigInt a = 1234;
                BigInt b = 5678;
                BigInt d = 3;
                BigInt c = 1;
                c = a + b + d;
                c = a + b + 2; // 1234 + 5678 = 6912
                c = c + a + 2; // 6912 + 1234 + 2 = 8148
                c = b + 2 + c; // 5678 + 2 + 8148 = 13828
                //c += 2; // 13828 + 2 = 13830
                c = c + 2;
                c++; // 13830 + 1 = 13831
                c = 100500 + c; // 100500 + 13831 = 114331
                std::cout << c << " ";
                return compare_couts(c, 114331);
                return true;
            }

            bool brainfuck_increments() {
                std::cout << "i=5; i = i++ + ++i";
                size_t i = 5;
                BigInt j = i;
                i = i++ + ++i;
                j = j++ + ++j;
                return compare_couts(j, i);
            }

            bool check_unary() {
                std::cout << "unary";
                BigInt i = -15;
                i = -i;
                return compare_couts(i, 15);
            }

            bool check_subtract() {
                std::cout << "subtract";
                BigInt a = 1234;
                BigInt b = 5678;
                BigInt c;
                c = b - a - b;
                return compare_couts(c, -a);
            }

            bool check_comparison1() {
                std::cout << "compare 1234 < 5678";
                BigInt a = 1234;
                BigInt b = 5678;
                return a < b;
            }

            bool check_comparison2() {
                std::cout << "compare -1234 < 5678";
                BigInt a = 1234;
                BigInt b = 5678;
                return -a < b;
            }

            bool check_comparison3() {
                std::cout << "compare -5678 < -1234";
                BigInt a = 1234;
                BigInt b = 5678;
                return -b < -a;
            }

            bool check_comparison4() {
                std::cout << "compare 5678 < 12345 ";
                BigInt a = 12345;
                BigInt b = 5678;
                return b < a;
            }

            std::vector<TestFunc> GetTests() {
                return {
                    //create_bigint,
                    //create_bigint_init_literal,
                    //cout_bigint,
                    //assign_int_to_bigint,
                    //assign_bigint_to_bigint,
                    adding_bigints,
                    brainfuck_increments,
                    check_unary,
                    check_subtract,
                    check_comparison1,
                    check_comparison2,
                    check_comparison3,
                    check_comparison4,
                };
            }
        }

        int RunTests(const TestGetter &tests_getter) {
            std::vector<TestFunc> tests = tests_getter();
            std::size_t tests_count = tests.size();
            bool failed = false;
            bool test_result = false;
            std::string error_msg;
            for (std::size_t i = 0; i < tests_count; ++i) {
                std::cout << "Running test " << i + 1 << "/" << tests_count << "... ";
                error_msg = "";
                try {
                    test_result = tests[i]();
                }
                catch (std::exception &e) {
                    error_msg = e.what();
                    test_result = false;
                }
                failed = failed || !test_result;
                if (test_result) {
                    std::cout << " OK";
                }
                else {
                    std::cerr << std::endl << "Failed!" << std::endl;
                    if (error_msg != "") {
                        std::cerr << error_msg;
                    }
                }
                std::cout << std::endl;
            }
            if (!failed) {
                std::cout << "All tests passed" << std::endl;
            }
            else {
                std::cout << "Some tests failed, check log for more details" << std::endl;
            }
            return 0;
        }
    }

}

int main() {
    made::test::RunTests(made::test::long_arithmetic::GetTests);
}