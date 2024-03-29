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

            bool get_rows() {
                std::cout << "Getting rows count";
                Matrix matrix(3, 4);
                if (matrix.rows() != 3)
                    return false;
                return true;
            }

            bool get_cols() {
                std::cout << "Getting columns count";
                Matrix matrix(4, 8);
                if (matrix.cols() != 8)
                    return false;
                return true;
            }

            bool check_index_access() {
                std::cout << "Checking index access";
                Matrix matrix(4, 8);
                matrix[1][0] = 5;
                matrix[1][0] = 9;
                if (matrix[1][0] != 9)
                    return false;
                return true;
            }

            bool check_index_access_out_of_rows_range() {
                std::cout << "Checking index access out of rows range";
                Matrix matrix(4, 8);
                try {
                    matrix[4][0] = 5;
                }
                catch (std::out_of_range _) {
                    return true;
                }
                return false;
            }

            bool check_index_access_out_of_cols_range() {
                std::cout << "Checking index access out of cols range";
                Matrix matrix(4, 8);
                try {
                    matrix[3][8] = 5;
                }
                catch (std::out_of_range _) {
                    return true;
                }
                return false;
            }

            bool check_multiplier() {
                std::cout << "Checking multiply by number";
                Matrix matrix(4, 8);
                matrix.Clear();
                for (int i = 0; i < 4; ++i) {
                    matrix[i][i] = i + 1;
                }
                matrix *= 8;
                for (int i = 0; i < 4; ++i) {
                    if (matrix[i][i] != (i + 1) * 8) {
                        return false;
                    }
                }
                return true;
            }

            bool check_equality() {
                std::cout << "Checking equality";
                Matrix matrix1(4, 8);
                Matrix matrix2(4, 8);
                matrix1.Clear();
                matrix2.Clear();
                for (int i = 0; i < 4; ++i) {
                    matrix1[i][i] = i + 1;
                    matrix2[i][i] = (i + 1) * 2;
                }
                matrix1 *= 2;
                return matrix1 == matrix2;;
            }

            bool check_unequality_size() {
                std::cout << "Checking unequality by size";
                Matrix matrix1(4, 8);
                Matrix matrix2(3, 4);
                return matrix1 != matrix2;
            }

            bool check_unequality_dimension() {
                std::cout << "Checking unequality by dimension";
                Matrix matrix1(4, 8);
                Matrix matrix2(8, 4);
                return matrix1 != matrix2;
            }

            bool check_unequality_by_values() {
                std::cout << "Checking unequality by values";
                Matrix matrix1(4, 8);
                Matrix matrix2(4, 8);
                matrix1.Clear();
                matrix2.Clear();
                for (int i = 0; i < 4; ++i) {
                    matrix1[i][i] = i + 1;
                    matrix2[i][i] = i + 1;
                }
                matrix1[3][7] = matrix2[3][7] * 2 + 8;
                return matrix1 != matrix2;
            }
        }

        std::vector<TestFunc> GetTests() {
            using namespace matrix;
            return {
                create_matrix,
                get_cols,
                get_rows,
                check_index_access,
                check_index_access_out_of_rows_range,
                check_index_access_out_of_cols_range,
                check_multiplier,
                check_equality,
                check_unequality_size,
                check_unequality_dimension,
                check_unequality_by_values,
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