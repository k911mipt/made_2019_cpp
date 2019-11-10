#include <vector>
#include <iostream>
#include <string>

#include "matrix.h"

namespace made {

    namespace test {
        typedef bool(*TestFunc)();
        typedef std::vector<TestFunc>(*TestGetter)();

        namespace matrix {
            using namespace made::math;

            bool create_matrix() {
                std::cout << "Creating matrix 4x4";
                Matrix matrix(4, 4);
                return true;
            }
        }

        std::vector<TestFunc> GetTests() {
            using namespace matrix;
            return {
                create_matrix,
            };
        }

        int RunTests(TestGetter tests_getter) {
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
    made::test::RunTests(made::test::GetTests);
}