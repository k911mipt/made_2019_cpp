#pragma once
#ifndef THREAD_POOL_TESTS_H_
#define THREAD_POOL_TESTS_H_

#include <functional>
#include <vector>

#include "common.h"
#include "thread_pool.hpp"

_MADE_BEGIN
_TEST_BEGIN

using TestFunc = std::function<bool()>;
using TestGetter = std::function<std::vector<TestFunc>()>;

_MULTITHREADING_BEGIN

std::vector<TestFunc> GetTests();


_MULTITHREADING_END
_TEST_END
_MADE_END

#endif // !PRINT_TESTS_H_