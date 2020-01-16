#include <iostream>

#include "thread_pool_tests.hpp"

_MADE_BEGIN
_TEST_BEGIN
_MULTITHREADING_BEGIN

using namespace made::multithreading;

struct A {};

void foo(const A& a) {}

bool thread_sample() {
    std::cout << "running sample thread ";

    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();
    auto task2 = pool.exec([]() {return 1; });
    return task2.get() == 1;
}

bool multiply_chain() {
    std::cout << "running task chain";
    ThreadPool pool(2);
    auto task1 = pool.exec([]() {return 5; });
    auto task2 = pool.exec([&task1]() {return 5 * task1.get(); });
    auto task3 = pool.exec([&task2]() {return 5 * task2.get(); });
    return task3.get() == 125;
}

std::vector<TestFunc> GetTests() {
    return {
        thread_sample,
        multiply_chain,
    };
}

_MULTITHREADING_END
_TEST_END
_MADE_END