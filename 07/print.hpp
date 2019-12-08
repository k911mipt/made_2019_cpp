#pragma once
#ifndef PRINT_H_
#define PRINT_H_

#include <string>
#include <sstream>
#include <vector>

namespace made {

    namespace print {

        class Formatter {
            inline static const std::string MSG_ERR_UNEXPECTED_END_OF_PLACEHOLDER = "unexpected end of placeholder";
            inline static const std::string MSG_ERR_UNEXPECTED_SYMBOL_AT_PLACEHOLDER = "unexpected symbol at placeholder";
            inline static const std::string MSG_ERR_UNEXPECTED_CLOSING_BRACKET = "unexpected closing bracket '}'";
            inline static const std::string MSG_ERR_TOO_FEW_ARGUMENTS = "too few arguments";
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

        template <class T>
        inline void Formatter::PushArgs(T&& value) {
            std::stringstream stream;
            stream << value;
            args_.push_back(stream.str());
        }

        template <class T, class... TArgs>
        inline void Formatter::PushArgs(T&& value, TArgs&&... args) {
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