#pragma once
#ifndef PRINT_TESTS_H_
#define PRINT_TESTS_H_

#include <algorithm>
#include <functional>

#include "vector.hpp"

namespace made {
    namespace test {
        using TestFunc = std::function<bool()>;
        using TestGetter = std::function<std::vector<TestFunc>()>;

        namespace stl {
            using namespace made::stl;

            class A {
            public:
                int x;
                A(int _x) :x(_x) {}
            };

            class B {
            public:
                inline static int counter = 0;
                int x;
                B() :x(counter++) {}
                B(int _x) :x(_x) {}
            };

#pragma region typed_tests
            template <class T>
            bool create_vector_without_arguments() {
                std::cout << "Creating Vector<" << typeid(T).name() << ">";
                Vector<T> v;
                return true;
            }

            template<class T, typename std::enable_if<std::is_default_constructible<T>::value, T>::type * = nullptr>
            bool create_vector_with_count() {
                std::cout << "Creating Vector<" << typeid(T).name() << ">(int count)";
                Vector<T> v(5);
                return true;
            }

            template<class T, typename std::enable_if<!std::is_default_constructible<T>::value, T>::type * = nullptr>
            bool create_vector_with_count() {
                std::cout << "Creating Vector<" << typeid(T).name() << ">(int count)";
                std::cout << ": " << typeid(T).name() << " has no default constructor, skipping";
                return true;
            }

            template<class T, typename std::enable_if<std::is_default_constructible<T>::value, T>::type * = nullptr>
            bool create_vector_with_init_value() {
                std::cout << "Creating Vector<" << typeid(T).name() << "> with init_value";
                Vector<T> v(5, T());
                return true;
            }

            template<class T, typename std::enable_if<!std::is_default_constructible<T>::value, T>::type * = nullptr>
            bool create_vector_with_init_value() {
                std::cout << "Creating Vector<" << typeid(A).name() << "> with init_value";
                Vector<A> v(5, A(4));
                return true;
            }

            template <class T>
            bool create_vector_with_init_list() {
                std::cout << "Creating Vector<" << typeid(T).name() << "> with initializer list";
                Vector<T> v{ 1,2,3 };
                return true;
            }

            template <class T>
            std::vector<TestFunc> build_typed_test_functions() {
                return {
                    create_vector_without_arguments<T>,
                    create_vector_with_count<T>,
                    create_vector_with_init_value<T>,
                    create_vector_with_init_list<T>,
                };
            }
#pragma endregion typed_tests

#pragma region other_tests
            bool sort_using_iterators() {
                std::cout << "sorting vector<int>{7, 3, 7, 56, 9, 2} using iterators";
                Vector<int> v{ 7, 3, 7, 56, 9, 2 };
                std::sort(v.begin(), v.end());
                for (auto it = v.begin() + 1; it < v.end(); ++it)
                    if (it < it - 1)
                        return false;
                return true;
            }

            bool check_push_back() {
                std::cout << "testing push_back";
                Vector<B> v;
                B b;
                v.push_back(b);
                v.push_back(b);
                v.push_back(b);
                return b.x == v.back().x;
            }

            bool check_emplace_back() {
                std::cout << "testing emplace_back";
                Vector<B> v;
                B b;
                v.push_back(b);
                v.push_back(b);
                v.emplace_back();
                return b.x == v.back().x - 1;
            }

            bool check_emplace() {
                std::cout << "testing emplace_back";
                Vector<A> v{ 1, 3, 4 };
                v.emplace(v.begin() + 1, 2);
                return v[1].x == 2 && v[3].x == 4;
            }

            bool check_reserve() {
                std::cout << "testing reserve";
                Vector<A> v{ 1, 3, 4 };
                v.reserve(4);
                return v[1].x == 3 && v[2].x == 4 && v.capacity() == 4 && v.size() == 3;
            }

            bool check_resize() {
                std::cout << "testing resize";
                Vector<A> v{ 1, 3, 4 };
                A a(10);
                v.resize(15, a);
                return v[1].x == 3 && v[2].x == 4 && v[14].x == 10;
            }

            bool check_resize_constructing() {
                std::cout << "testing resize constructing single time";
                B::counter = 9;
                Vector<B> v{ 1, 3, 4, 2, 0 };
                v.resize(15);
                return v[1].x == 3 && v[13].x == 9 && v[14].x == 9;
            }

            bool check_pop_back() {
                std::cout << "testing pop_back";
                Vector<B> v{ 1, 3, 4, 2, 0 };
                v.resize(15);
                v.pop_back();
                return v.size() == 14;
            }

            bool check_erase() {
                std::cout << "testing erase";
                Vector<B> v{ 1, 3, 4, 2, 0 };
                v.erase(v.begin() + 1, v.end() - 1);
                return v.size() == 2 && v.front().x == 1 && (v.end() - 1)->x == 0;
            }

            std::vector<TestFunc> get_other_test_functions() {
                return {
                    sort_using_iterators,
                    check_push_back,
                    check_emplace_back,
                    check_emplace,
                    check_reserve,
                    check_resize,
                    check_resize_constructing,
                    check_pop_back,
                    check_erase
                };
            }
#pragma endregion other_tests

            template <typename T>
            struct type_wrapper { using type = T; };

            template <typename... Ts, typename TF>
            void for_types(TF&& f) { (f(type_wrapper<Ts>{}), ...); }

            template <typename... Ts>
            std::vector<TestFunc> get_type_shared_tests() {
                std::vector<TestFunc> result;
                for_types<Ts...>([&result](auto t)
                    {
                        using T = typename decltype(t)::type;
                        auto temp = build_typed_test_functions<T>();
                        result.insert(result.end(), temp.begin(), temp.end());
                    });
                return result;
            }

            std::vector<TestFunc> GetTests() {
                std::vector<TestFunc> result = get_type_shared_tests<int, A, B>();
                std::vector<TestFunc> other = get_other_test_functions();
                result.insert(result.end(), other.begin(), other.end());
                return result;
            }
        }
    }
}

#endif // !PRINT_TESTS_H_