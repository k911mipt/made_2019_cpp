#pragma once
#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <ostream>
#include <type_traits>

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>

namespace made {

    namespace stl {
        class OutOfMemory : public std::exception {
        public:
            OutOfMemory() noexcept : std::exception() {}
            const char* what() { return "out of memory"; }
        };
    }

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
        //const size_t MAX_DOUBLING_SIZE = 1 << 28;

        template <class T>
        class Container {
            //using T = size_t;// TODO remove it
            static const size_t MAX_DOUBLING_SIZE = 1 << 28 / sizeof(T); // 256 MB
            static const size_t SIZE_STEP = MAX_DOUBLING_SIZE >> 1; // 128 MB
        public:
            Container() = default;
            Container(size_t capacity);// :capacity_(capacity);
            Container(const Container& other) { *this = other; }
            Container& operator=(const Container& copied);
            Container& operator=(Container&& moved);
            ~Container() { delete[] buffer_; }
            T& operator[](size_t index) { return buffer_[index]; }
            void SetSize(size_t size);
            void TrimSize() { for (; (size_ > 0) && (0 == buffer_[size_ - 1]); --size_); }
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
            //public:
            size_t size_ = 0;
            friend class BigInt;
        };

#pragma region Container

        template <class T>
        Container<T>::Container(size_t capacity) : capacity_(capacity) {
            buffer_ = new T[capacity];
            std::memset(buffer_, 0, sizeof(T) * capacity);
        }

        template <class T>
        Container<T>& Container<T>::operator=(const Container<T>& copied) {
            if (this == &copied) {
                return *this;
            }
            size_t capacity = copied.capacity_;
            T* ptr = new T[capacity];
            delete[] buffer_;
            buffer_ = ptr;
            capacity_ = capacity;
            size_ = copied.size_;
            std::copy(copied.buffer_, copied.buffer_ + capacity_, buffer_);
            return *this;
        }
        template <class T>
        Container<T>& Container<T>::operator=(Container<T>&& moved) {
            if (this == &moved) {
                return *this;
            }
            delete[] buffer_;
            buffer_ = moved.buffer_;
            moved.buffer_ = nullptr;
            capacity_ = moved.capacity_;
            size_ = moved.size_;
            return *this;
        }

        template <class T>
        void Container<T>::SetSize(size_t size) {
            size_t capacity = capacity_;
            while ((size > capacity) && (capacity < MAX_DOUBLING_SIZE)) {
                capacity *= 2;
            }
            while ((size > capacity) && (capacity < SIZE_MAX - SIZE_STEP)) {
                capacity += SIZE_STEP;
            }
            if (size > capacity) {
                throw stl::OutOfMemory();
            }
            T* ptr = new T[capacity];
            std::copy(buffer_, buffer_ + size_, ptr);
            std::memset(ptr + size_, 0, sizeof(T) * (capacity - size_));
            delete[] buffer_;
            buffer_ = ptr;
            size_ = size;
        }
#pragma endregion 


        class BigInt {
            const size_t BASE = 100;
            using base_t = uint32_t;
            const size_t INIT_SIZE = size_t(log10(SIZE_MAX) / log10(BASE)) + 1;
        public:
            BigInt() = default;
            template<typename Tint>
            BigInt(const Tint number) : sign(number >= 0) {
                size_t module = abs(number);
                size_t next = 0;
                while (module) {
                    digits[next++] = (base_t)(module % BASE);
                    module /= BASE;
                }
                digits.size_ = next;
                //digits.SetSize(next + 20);
            }
            BigInt(const BigInt& other) { *this = other; }
            BigInt& operator=(const BigInt& copied) {
                if (this == &copied) {
                    return *this;
                }
                sign = copied.sign;
                digits = copied.digits;
                return *this;
            }
            BigInt& operator=(BigInt&& moved) {
                if (this == &moved) {
                    return *this;
                }
                sign = moved.sign;
                digits = std::move(moved.digits);
                return *this;
            }
            ~BigInt() = default;

            void AddAbs(const BigInt& other) {
                size_t old_size = digits.size_;
                digits.SetSize(std::max(digits.size_, other.digits.size_) + 1);
                unsigned int carry = 0;
                size_t min_size = std::min(digits.size_, other.digits.size_);
                base_t* a = digits.buffer_;
                base_t* b = other.digits.buffer_;
                for (size_t i = 0; i < min_size; ++i, ++a, ++b) {
                    unsigned int temp_carry = (BASE - *b - carry) <= *a;
                    *a = (*a + *b + carry) % BASE;
                    carry = temp_carry;
                    //digits[i] = (digits[i] + copied.digits[i]) % BASE;
                }
                b = old_size < other.digits.size_ ? b : a;
                //for (size_t i = min_size; (i < digits.size_) && carry; ++i) {
                for (size_t i = min_size; (i < digits.size_); ++i) {
                    unsigned int temp_carry = (BASE - carry) <= *a;
                    *a = (*b + carry) % BASE;
                    carry = temp_carry;
                }
                digits.TrimSize();
                //for (size_t)
            }

            void operator+=(const BigInt& other) {
                if (sign == other.sign) {
                    AddAbs(other);
                }
            }

            const BigInt operator+(const BigInt& other) {
                BigInt result(*this);
                result += other;
                return result;
            }

            template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type>
            const BigInt operator+(const Tint& other) {
                BigInt result(*this);
                result += BigInt(other);
                return result;
            }

            //template<class Tint>
            //friend const BigInt operator+(const BigInt& lhs, Tint& rhs) {
            //    BigInt b_rhs(rhs);
            //    //result += BigInt(copied);
            //    return lhs + b_rhs;
            //}

            friend const BigInt operator+(BigInt& lhs, int rhs) {
                BigInt b_rhs(rhs);
                //result += BigInt(copied);
                return lhs + b_rhs;
            }

            friend const BigInt operator+(BigInt& lhs, BigInt& rhs) {
                BigInt result(lhs);
                result += rhs;
                return result;
            }

            friend std::ostream& operator<<(std::ostream& out, const BigInt& number);
        private:
            bool sign;
            Container<base_t> digits = Container<base_t>(INIT_SIZE);
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