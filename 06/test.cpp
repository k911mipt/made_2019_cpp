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

        namespace serializer {
            using namespace made::serializer;

            struct Data {
                uint64_t a;
                bool b;
                uint64_t c;

                template <class Serializer>
                Error serialize(Serializer& serializer) {
                    return serializer(a, b, c);
                }
            };

            bool create_serializer() {
                std::cout << "serializer";
                std::stringstream stream;
                Serializer serializer(stream);
                return true;
            }

            bool check_serializer_save_execute() {
                std::cout << "serializing data";
                std::stringstream stream;
                Serializer serializer(stream);
                Data data{ 0, true, 1 };
                return serializer.save(data) == Error::NoError;
            }

            bool check_serializer_save_execute_correct() {
                std::cout << "serializing \"0 true 1\"";
                std::stringstream stream;
                Serializer serializer(stream);
                Data data{ 0, true, 1 };
                serializer.save(data);
                return stream.str() == "0 true 1";
            }

            bool check_deserializer_save_execute_correct() {
                std::cout << "deserializing \"0 true 1\"";
                std::stringstream stream;
                stream.str("0 true 1");
                Deserializer deserializer(stream);
                Data data;
                Error error = deserializer.load(data);
                return ((error == Error::NoError) && (data.a == 0) && (data.b) && (data.c == 1));
            }

            bool check_deserializer_save_execute_uncorrect() {
                std::cout << "deserializing \"0o true 1\"";
                std::stringstream stream;
                stream.str("0o true 1");
                Deserializer deserializer(stream);
                Data data;
                return (deserializer.load(data) == Error::CorruptedArchive);
            }
        }

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

            //template <typename T1, typename T2>
            //bool compare_couts(T1&& e1, T2&& e2) {
            //    return compare_couts_(std::forward<T1>(e1), std::forward<T2>(e2));
            //}

            bool create_bitint() {
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
                std::cout << "adding bigints";
                BigInt a = 1234;
                BigInt b = 5678;
                BigInt c;
                c = a + b;
                c = a + b + 2;
                //+2;
                //c = a + b;
                //BigInt c = a + 2;
                //return compare_couts(a, b);
                return compare_couts(c, 1234+5678 + 2);
                return true;
            }

            std::vector<TestFunc> GetTests() {
                return {
                    create_bitint,
                    create_bigint_init_literal,
                    cout_bigint,
                    assign_int_to_bigint,
                    assign_bigint_to_bigint,
                    adding_bigints,
                };
            }
        }

        //std::vector<TestFunc> GetTests() {
        //    using namespace serializer;
        //    return {
        //        create_serializer,
        //        check_serializer_save_execute,
        //        check_serializer_save_execute_correct,
        //        check_deserializer_save_execute_correct,
        //        check_deserializer_save_execute_uncorrect,
        //    };
        //}

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