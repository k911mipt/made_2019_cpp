#include <vector>
#include <iostream>
#include "tokenizer.h"

#include "linear_allocator.hpp"


namespace made {

    namespace test {
        typedef bool(*TestFunc)();
        typedef std::vector<TestFunc>(*TestGetter)();

        namespace parser {
            using namespace made::parser;
            typedef bool(*TestFunc)();

            namespace token_events {
                using namespace made::parser;
                void cb_check_called(Token value) {
                    std::cout << "Token callback called";
                }
            }

            bool create_parser() {
                std::cout << "Creating parser";
                Parser parser;
                return true;
            }

            bool parse_string() {
                std::cout << "Parsing empty string";
                Parser parser;
                parser.Parse("");
                return true;
            }

            bool parse_and_expect_one_token() {
                std::cout << "Getting number of parsed tokens, expecting 1";
                Parser parser;
                parser.Parse("abc");
                if (parser.GetTokens().size() != 1)
                    return false;
                return true;
            }

            bool parse_and_expect_string_token_parsed_as_string() {
                std::cout << "Expecting \"0abc\" to be parsed as string";
                Parser parser;
                parser.Parse("0abc");
                if (parser.GetTokens()[0].type != STRING)
                    return false;
                return true;
            }

            bool parse_and_expect_number_token_parsed_as_number() {
                std::cout << "Expecting \"073732\" to be parsed as number";
                Parser parser;
                parser.Parse("073732");
                if (parser.GetTokens()[0].type != NUMBER)
                    return false;
                return true;
            }
            
            bool parse_and_expect_2_tokens() {
                std::cout << "Getting number of parsed tokens, expecting 2";
                Parser parser;
                parser.Parse("0abc 073732");
                if (parser.GetTokens().size() != 2)
                    return false;
                return true;
            }

            bool register_callback() {
                std::cout << "Registering callback, expecting 2 calls";
                Parser parser;
                int x = 0;
                parser.RegisterTokenEvent([&x](Token _) { ++x; });
                parser.Parse("0abc 073732");
                if (x != 2)
                    return false;
                return true;
            }

            bool register_string_callback() {
                std::cout << "Registering string callback, expecting 1 calls";
                Parser parser;
                int x = 0;
                parser.RegisterCustomTokenEvent([&x](Token _) { ++x; }, TokenType::STRING);
                parser.Parse("0abc 073732");
                if (x != 1)
                    return false;
                return true;
            }

            bool register_number_callback() {
                std::cout << "Registering number callback, expecting 2 calls";
                Parser parser;
                int x = 0;
                parser.RegisterCustomTokenEvent([&x](Token _) { ++x; }, TokenType::NUMBER);
                parser.Parse("12 0abc 073732");
                if (x != 2)
                    return false;
                return true;
            }

            bool expect_sum_of_parsed_numbers_via_callback() {
                std::cout << "Registering number callback, expect count sum of numbers in data";
                Parser parser;
                int x = 0;
                parser.RegisterCustomTokenEvent([&x](Token token) { x += token.getNumber(); }, TokenType::NUMBER);
                parser.Parse("   12 0abc 073732   ");
                if (x != 73744)
                    return false;
                return true;
            }

            std::vector<TestFunc> GetTests() {
                return {
                    create_parser,
                    parse_string,
                    parse_and_expect_one_token,
                    parse_and_expect_string_token_parsed_as_string,
                    parse_and_expect_number_token_parsed_as_number,
                    parse_and_expect_2_tokens,
                    register_callback,
                    register_string_callback,
                    register_number_callback,
                    expect_sum_of_parsed_numbers_via_callback
                };
            }
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
                        std::cerr << error_msg;// << std::endl;
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
    made::test::RunTests(made::test::parser::GetTests);
    //made::stl::linear_allocator_tests::RunTests();
}