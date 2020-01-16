#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "serializer.hpp"


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

        std::vector<TestFunc> GetTests() {
            using namespace serializer;
            return {
                create_serializer,
                check_serializer_save_execute,
                check_serializer_save_execute_correct,
                check_deserializer_save_execute_correct,
                check_deserializer_save_execute_uncorrect,
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