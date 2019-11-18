#pragma once
#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <ostream>
#include <type_traits>

namespace made {

    namespace serializer {
        enum class Error
        {
            NoError,
            CorruptedArchive
        };

        class Serializer
        {
            static constexpr char Separator = ' ';
        public:
            explicit Serializer(std::ostream& out) : out_(out) {}

            template <class T>
            Error save(T& object) {
                return object.serialize(*this);
            }

            template <class... ArgsT>
            Error operator()(ArgsT... args) {
                auto flags = out_.flags();
                out_ << std::boolalpha; // interpret boolean as "true"/"false" string instead of "0"/"1"
                auto result = process(args...);
                out_.flags(flags);
                return result;
            }

        private:
            std::ostream& out_;

            template <class T>
            Error process(T& value) {
                return value.serialize(*this);
            }

            Error process(bool value) {
                out_ << value;
                return Error::NoError;
            }

            Error process(uint64_t value) {
                out_ << value;
                return Error::NoError;
            }

            template <class T, class... ArgsT>
            Error process(T& value, ArgsT&... args) {
                Error error = process(value);
                if (error != Error::NoError) {
                    return error;
                }
                out_ << Separator;
                return process(args...);
            }
        };

        class Deserializer {
        public:
            explicit Deserializer(std::istream& in) : in_(in) {}

            template <class T>
            Error load(T& object) {
                Error result = object.serialize(*this);
                if (!in_.eof()) return Error::CorruptedArchive;
                return result;
            }

            template <class... ArgsT>
            Error operator()(ArgsT&... args) {
                auto flags = in_.flags();
                in_ >> std::boolalpha; // interpret boolean as "true"/"false" string instead of "0"/"1"
                auto result = process(args...);
                in_.flags(flags);
                return result;
            }

        private:
            std::istream& in_;

            template <class T>
            Error process(T& value) {
                return value.serialize(*this);
            }

            Error process(bool& value) {
                in_ >> value;
                return Error::NoError;
            }

            Error process(uint64_t& value) {
                in_ >> value;
                return Error::NoError;
            }

            template <class T, class... ArgsT>
            Error process(T& value, ArgsT&... args) {
                Error error = process(value);
                if (error != Error::NoError) {
                    return error;
                }
                return process(args...);
            }
        };
    }
}

#endif  // !SERIALIZER_H_