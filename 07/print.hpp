#pragma once
#ifndef PRINT_H_
#define PRINT_H_

#include <string>
#include <sstream>
#include <stdexcept>

namespace made {

    namespace print {

        class Formatter {
            inline static const std::string  MSG_ERR_UNEXPECTED_END_OF_PLACEHOLDER = "unexpected end of placeholder";
            inline static const std::string  MSG_ERR_UNEXPECTED_SYMBOL_AT_PLACEHOLDER = "unexpected symbol at placeholder";
            inline static const std::string  MSG_ERR_UNEXPECTED_CLOSING_BRACKET = "unexpected closing bracket '}'";
            inline static const std::string  MSG_ERR_TOO_FEW_ARGUMENTS = "too few arguments";
        public:
            Formatter(const std::string& str) : source_(str) {}
            template <class T>
            void PushArgs(T&& value);
            template <class T, class... TArgs>
            void PushArgs(T&& value, TArgs&&... args);
            std::string Parse();
        private:
            std::string ParseArgNum(size_t& pos);

            std::string source_;
            std::vector<std::string> args_;
        };

        inline std::string Formatter::ParseArgNum(size_t& pos) {
            ++pos;
            if (pos == source_.size())
                throw std::runtime_error(MSG_ERR_UNEXPECTED_END_OF_PLACEHOLDER);
            char c = source_[pos];
            if (!isdigit(c))
                throw std::runtime_error(MSG_ERR_UNEXPECTED_SYMBOL_AT_PLACEHOLDER);
            size_t arg_num = 0;
            do {
                arg_num = arg_num * 10 + (c - '0');
                if (arg_num >= args_.size())
                    throw std::runtime_error(MSG_ERR_TOO_FEW_ARGUMENTS);
                ++pos;
                if (pos == source_.size())
                    throw std::runtime_error(MSG_ERR_UNEXPECTED_END_OF_PLACEHOLDER);
                c = source_[pos];
            } while (isdigit(c));
            if (c != '}')
                throw std::runtime_error(MSG_ERR_UNEXPECTED_END_OF_PLACEHOLDER);
            return args_[arg_num];
        }

        std::string Formatter::Parse() {
            std::string result;
            size_t pos = 0;
            while (pos < source_.size()) {
                char c = source_[pos];
                if (c == '}')
                    throw std::runtime_error(MSG_ERR_UNEXPECTED_CLOSING_BRACKET);
                if (c == '{')
                    result += ParseArgNum(pos);
                else
                    result += c;
                ++pos;
            }
            return result;
        }

        template <class T>
        void Formatter::PushArgs(T&& value) {
            std::stringstream stream;
            stream << value;
            args_.push_back(stream.str());
        }

        template <class T, class... TArgs>
        void Formatter::PushArgs(T&& value, TArgs&&... args) {
            PushArgs(std::forward<T>(value));
            PushArgs(std::forward<TArgs>(args)...);
        }

        template <class... TArgs>
        std::string format(const std::string& str, TArgs&&... args) {
            Formatter formatter(str);
            formatter.PushArgs(std::forward<TArgs>(args)...);
            return formatter.Parse();
        }
    }
}

#endif  // !PRINT_H_