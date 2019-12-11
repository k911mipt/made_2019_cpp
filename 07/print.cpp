#include "print.hpp"

namespace made {

    namespace print {

        std::string Formatter::ParseArgNum(size_t& pos) {
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

    }
}