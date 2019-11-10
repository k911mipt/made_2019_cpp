#pragma once
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <variant>
#include <cctype>
#include <cassert>

using std::string;

namespace made {

    namespace parser {
        using std::string;
        enum TokenType {
            NUMBER,
            STRING
        };

        //struct TokenType {
        //    TokenType type;
        //    //if type == Number
        //    //    int field
        //    //else 
        //    //    string field

        //};

        struct TokenNumber {
        public:
            int value;
            const TokenType type = NUMBER;
            //    TokenType GetType() { return type; }
            //private:
            //    TokenType type = NUMBER;
        };

        struct TokenString {
        public:
            string value;
            const TokenType type = STRING;
            //    TokenType GetType() { return type; }
            //private:
            //    TokenType type = STRING;
        };
        class Tokenizer {
        public:
            Tokenizer(std::string line) {

            }
        private:
            void Parse() {

            }

        };

        class Token {
        public:
            //Token(TokenType initial_type) :type(initial_type) {}
            //Token(const std::string &init_data, TokenType tokenType): data(init_data), type(DetermineTokenType(init_data)) {}
            Token(const std::string &init_data, TokenType tokenType) : data(init_data), type(tokenType) {}
            const TokenType type;
        private:
            const std::string data;
            int number;
            TokenType DetermineTokenType(const std::string &init_data);
        };

        TokenType Token::DetermineTokenType(const std::string &init_data) {
            try {
                size_t pos = 0;
                number = std::stoi(data, &pos);
                return (pos < data.length()) ? STRING : NUMBER;
            }
            catch (std::exception _) {
                return STRING;
            }
        }


        class Parser {
        public:
            void Parse(std::string &&line) {
                if (line.size() == 0) {
                    return;
                }
                size_t begin, end;
                end = begin = 0;
                while (begin < line.size()) {
                    SkipSpaces(line, begin);
                    end = begin;
                    TokenType tokenType = SkipNonSpaces(line, end);
                    const std::string str = line.substr(begin, end - begin);
                    tokens_.push_back(Token(str, tokenType));
                    begin = end;
                }
            }
            const std::vector<Token> GetTokens() {
                return tokens_;
            }
        private:
            std::vector<Token> tokens_;
            void SkipSpaces(std::string &line, size_t &index) {
                assert(index < line.size());
                for (; (index < line.size()) && std::isspace(line[index]); ++index);
            }
            TokenType SkipNonSpaces(std::string &line, size_t &index) {
                assert(index < line.size());
                bool result = true;
                index += line[index] == '-'; // skip unary minus
                for (; (index < line.size()) && !std::isspace(line[index]); ++index) {
                    result = !result || (('0' <= line[index]) && line[index] <= '9');
                };
                return result ? NUMBER : STRING;
            }
        };
        /*
        Написать библиотеку - парсер строк состоящих из следующих токенов :

        * строка
        * число
        Число состоит из символов от 0 до 9, строка - все остальные символы.Токены разделены пробелами, символами табуляции и первода строки.

        Пользователь библиотеки должен иметь возможность зарегистрировать колбек - функцию вызываемую каждый раз, когда найден токен, функция получает токен.Должно быть возможно зарегистрировать свой обработчик под каждый тип токена.Также должны быть колбеки вызываемые перед началом парсинга и по его окончанию.
        */
    }
}
#endif  // !TOKENIZER_H_