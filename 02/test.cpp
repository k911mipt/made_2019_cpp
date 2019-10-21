#include <vector>
#include <iostream>
#include "linear_allocator.hpp"

namespace made {

    namespace stl {

        namespace linear_allocator_tests {

            typedef void(*TestFunc)();

            struct TestCase {
                TestFunc function;
                bool expect_out_of_memory;
            };

            namespace valid_oom {

                void t1() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(11);
                }

                void t2() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                }

                void t3() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(1);
                    allocator.Reset();
                    p = allocator.Alloc(11);
                }
            }

            namespace valid_reset {

                void t1() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(10);
                    allocator.Reset();
                    p = allocator.Alloc(10);
                }

                void t2() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(1);
                }

                void t3() {
                    LinearAllocator<int> allocator(10);
                    int *p = allocator.Alloc(3);
                    allocator.Reset();
                    p = allocator.Alloc(3);
                    allocator.Reset();
                    p = allocator.Alloc(3);
                    allocator.Reset();
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(3);
                    p = allocator.Alloc(1);
                    allocator.Reset();
                    p = allocator.Alloc(10);
                }
            }

            namespace valid_consistency {

                void t1() {
                    LinearAllocator<int> allocator(10);
                    // p2 is allocated right after p1.
                    // Fill p2 with some values,
                    // then address out of borders of p1,
                    // assuming it will point to p2 values
                    // due to linearity consistentcy
                    int* p1 = allocator.Alloc(5);
                    int* p2 = allocator.Alloc(5);
                    for (int i = 0; i < 5; i++) {
                        p2[i] = i;
                    }
                    for (int i = 0; i < 5; i++) {
                        if (p1[i + 5] != i) {
                            throw std::exception("linearity is not consistent");
                        }
                    }
                }

                void t2() {
                    LinearAllocator<int> allocator(10);
                    // Assume reseting doesnt changes state of block
                    int* p1 = allocator.Alloc(10);
                    for (int i = 0; i < 10; i++) {
                        p1[i] = i;
                    }
                    allocator.Reset();
                    for (int i = 0; i < 10; i++) {
                        if (p1[i] != i) {
                            throw std::exception("memory block has been changed with reseting allocator");
                        }
                    }
                }

                void t3() {
                    LinearAllocator<int> allocator(10);
                    // Assume reseting doesnt allocate a new block
                    int* p1 = allocator.Alloc(10);
                    for (int i = 0; i < 10; i++) {
                        p1[i] = i;
                    }
                    allocator.Reset();
                    int* p2 = allocator.Alloc(10);
                    for (int i = 0; i < 10; i++) {
                        if (p1[i] != p2[i]) {
                            throw std::exception("memory block has been reallocated with reseting allocator");
                        }
                    }
                }
            }

            std::vector<TestCase> GetTests() {
                return {
                    { valid_oom::t1, true },
                    { valid_oom::t2, true },
                    { valid_oom::t3, true },
                    { valid_reset::t1, false },
                    { valid_reset::t2, false },
                    { valid_reset::t3, false },
                    { valid_consistency::t1, false },
                    { valid_consistency::t2, false },
                    { valid_consistency::t3, false },
                };
            }

            bool RunTest(TestCase test) {
                try {
                    if (!test.expect_out_of_memory) {
                        test.function();
                        return true;
                    }
                    try {
                        test.function();
                    }
                    catch (made::stl::OutOfMemory _) {
                        return true;
                    }
                    std::cerr << "Failed" << std::endl;
                    std::cerr << "Expected out of memory" << std::endl;
                    return false;
                }
                catch (std::exception &e) {
                    std::cerr << "Failed" << std::endl;
                    std::cerr << e.what() << std::endl;
                    return false;
                }
            }

            int RunTests() {
                std::vector<TestCase> tests = GetTests();
                std::size_t tests_count = tests.size();
                bool failed = false;
                for (std::size_t i = 0; i < tests_count; ++i) {
                    std::cout << "Running test " << i + 1 << "/" << tests_count << "... ";
                    bool test_result = RunTest(tests[i]);
                    failed = failed || !test_result;
                    if (test_result) {
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

    }

}

int main() {
    made::stl::linear_allocator_tests::RunTests();
}