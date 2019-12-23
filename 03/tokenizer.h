#pragma once
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <functional>
#include <stdexcept>

namespace made {

    namespace parser {
        enum TokenType { // Must not specify numbers in this enum, its is used as an array indexer!
            NUMBER,
            STRING,
            COUNT // Last element = size of enum
        };

        class Token {
        public:
            Token(const std::string& init_data, TokenType tokenType) : data_(init_data), type(tokenType), number_(std::atoi(data_.c_str())) {
            }
            const TokenType type;
            const int getNumber() {
                if (type != NUMBER) {
                    throw std::domain_error("can't get number from a string token");
                }
                return number_;
            }
            const std::string& getData() {
                return data_;
            }
        private:
            const std::string data_;
            const int number_;
        };

        typedef std::function<void(Token)> TokenEvent;
        typedef std::function<void()> ParsingEvent;

        class Parser {
        public:
            void Parse(std::string&& line);
            const std::vector<Token> GetTokens() { return tokens_; }
            void RegisterTokenEvent(TokenEvent cb) { tokensEvents_.push_back(cb); }
            void RegisterCustomTokenEvent(TokenEvent cb, TokenType tokenType) { tokensCustomEvents_[tokenType].push_back(cb); }
            void RegisterBeforeParsingEvent(ParsingEvent& cb) { beforeEvents_.push_back(cb); }
            void RegisterBeforeParsingEvent(ParsingEvent&& cb) { beforeEvents_.push_back(cb); }
            void RegisterAfterParsingEvent(ParsingEvent& cb) { afterEvents_.push_back(cb); }
            void RegisterAfterParsingEvent(ParsingEvent&& cb) { afterEvents_.push_back(cb); }
        private:
            std::vector<Token> tokens_;
            std::vector<ParsingEvent> beforeEvents_;
            std::vector<ParsingEvent> afterEvents_;
            std::vector<TokenEvent> tokensEvents_;
            std::vector<TokenEvent> tokensCustomEvents_[TokenType::COUNT];
            void ProcessTokenEvents(Token token) {
                for (auto cb : tokensEvents_) { cb(token); }
                for (auto cb : tokensCustomEvents_[token.type]) { cb(token); }
            }
            void ProcessBeforeParsingEvents() { for (auto cb : beforeEvents_) { cb(); } }
            void ProcessAfterParsingEvents() { for (auto cb : afterEvents_) { cb(); } }
            void SkipSpaces(std::string& line, size_t& index) { for (; (index < line.size()) && std::isspace(line[index]); ++index); }
            TokenType SkipNonSpaces(std::string& line, size_t& index);
        };

        void Parser::Parse(std::string&& line) {
            ProcessBeforeParsingEvents();
            if (line.size() == 0) {
                return;
            }
            size_t begin, end;
            end = begin = 0;
            SkipSpaces(line, begin);
            while (begin < line.size()) {
                end = begin;
                TokenType tokenType = SkipNonSpaces(line, end);
                Token token = Token(line.substr(begin, end - begin), tokenType);
                tokens_.push_back(token);
                ProcessTokenEvents(token);
                begin = end;
                SkipSpaces(line, begin);
            }
            ProcessAfterParsingEvents();
        }

        TokenType Parser::SkipNonSpaces(std::string& line, size_t& index) {
            assert(index < line.size());
            bool result = true;
            index += line[index] == '-'; // skip unary minus
            for (; (index < line.size()) && !std::isspace(line[index]); ++index) {
                result = !result || (('0' <= line[index]) && line[index] <= '9');
            };
            return result ? NUMBER : STRING;
        }
    }
}
#endif  // !TOKENIZER_H_