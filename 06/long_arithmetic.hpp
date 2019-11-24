#pragma once
#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <ostream>
#include <type_traits>

#include <cstdlib>
#include <cstring>

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

    namespace long_arithmetic {
        const int tab32[32] = {
             0,  9,  1, 10, 13, 21,  2, 29,
            11, 14, 16, 18, 22, 25,  3, 30,
             8, 12, 20, 28, 15, 17, 24,  7,
            19, 27, 23,  6, 26,  5,  4, 31 };
        inline int log2_32(uint32_t value) {
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            return tab32[(uint32_t)(value * 0x07C4ACDD) >> 27];
        }

        template <class T>
        class Container {
            //using T = size_t;// TODO remove it
        public:
            Container() = default;
            Container(size_t capacity) :capacity_(capacity) {
                buffer_ = new T[capacity];
                //std::memset(buffer_, 0, sizeof(buffer_) * capacity);
                std::memset(buffer_, 0, sizeof(T) * capacity);
            }
            Container(const Container& other) {
                //if (this == &other) {
                //    return;
                //}
                //size_t capacity = other.capacity_;
                //T* ptr = new T[capacity];
                //capacity_ = capacity;
                //size_ = other.size_;
                //std::copy(other.buffer_, other.buffer_ + capacity_, buffer_);
                *this = other;
            }
            Container& operator=(const Container& other) {
                if (this == &other) {
                    return *this;
                }
                size_t capacity = other.capacity_;
                T* ptr = new T[capacity];
                delete[] buffer_;
                buffer_ = ptr;
                capacity_ = capacity;
                size_ = other.size_;
                std::copy(other.buffer_, other.buffer_ + capacity_, buffer_);
                return *this;
                // TODO no need to make same capacity when copying
            }
            ~Container() {
                delete[] buffer_;
            }
            T &operator[](size_t index) {
                return buffer_[index];
            }
            friend std::ostream& operator<<(std::ostream& out, const Container& container) {
                for (size_t i = container.size_ - 1; i > 0; --i) {
                    out << container.buffer_[i];
                }
                out << container.buffer_[0];
                return out;
            }
        private:
            size_t capacity_ = 0;
            T* buffer_;
        public:
            size_t size_ = 0;
        };

        //template <class T>
        //std::ostream& operator<<(std::ostream& out, const Container<T>& buffer) {
        //for (size_t i = 0; i < container.size_; ++i) {
        //    out << container.buffer_[i];
        //}
        //return out;
        //}

        class BigInt {
            const size_t BASE = 10;
            const size_t INIT_SIZE = size_t(log10(SIZE_MAX) / log10(BASE)) + 1;
        public:
            BigInt() = default;
            template<typename Tint>
            BigInt(Tint number) : sign(number >= 0) {
                size_t module = abs(number);
                size_t next = 0;
                while (module) {
                    digits[next++] = module % BASE;
                    module /= BASE;
                }
                digits.size_ = next;
            }
            ~BigInt() = default;

            friend std::ostream& operator<<(std::ostream& out, const BigInt& number);
        private:
            bool sign;
            Container<uint32_t> digits = Container<uint32_t>(INIT_SIZE);
            //Container digits(INIT_SIZE);// = Container;
        };



        std::ostream& operator<<(std::ostream& out, const BigInt& number) {
            std::string result;
            if (!number.sign) {
                out << "-";
            }
            out << number.digits;
            return out;
        }

    }
}

#endif  // !SERIALIZER_H_