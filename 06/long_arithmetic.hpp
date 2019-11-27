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

        template< typename T >
        T Abs(T val) {
            if constexpr (std::is_signed_v<T>)
                return((val <= -1) ? -val : val);
            else if constexpr (std::is_unsigned_v<T>)
                return val;
            else
                static_assert(false, "Unsupported type");
        }

        template <typename T> int Sign(T val) {
            return (T(0) < val) - (val < T(0));
        }

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
    }

    namespace long_arithmetic {

        //const size_t MAX_DOUBLING_SIZE = 1 << 28;

        template <class T>
        class Container {
            //using T = size_t;// TODO remove it
            static const size_t MAX_DOUBLING_SIZE = 1 << 28 / sizeof(T); // 256 MB
            static const size_t SIZE_STEP = MAX_DOUBLING_SIZE >> 1; // 128 MB
        public:
            Container() = default;
            Container(size_t capacity);// :capacity_(capacity);
            Container(const Container& copied);
            Container(Container&& moved);
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
            size_t size_ = 0;
            T* buffer_;
            friend class BigInt;
        };

#pragma region Container

        template <class T>
        Container<T>::Container(size_t capacity) : capacity_(capacity) {
            buffer_ = new T[capacity];
            std::memset(buffer_, 0, sizeof(T) * capacity);
        }

        template<class T>
        inline Container<T>::Container(const Container& copied) :
            capacity_(copied.capacity_),
            size_(copied.size_)
        {
            buffer_ = new T[capacity_];
            std::copy(copied.buffer_, copied.buffer_ + capacity_, buffer_);
        }

        template<class T>
        inline Container<T>::Container(Container&& moved) :
            buffer_(moved.buffer_),
            capacity_(moved.capacity_),
            size_(moved.size_)
        {
            moved.buffer_ = nullptr;
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
            if (size == size_) return;
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
            using base_t = uint32_t;
            static const base_t BASE = 10; //1e9
            //const base_t MAX_BASE_TYPE_VALUE = (~0 >> 1) - 1 ;
            // TODO static assert size;
            //using base_t = int32_t;
            const size_t INIT_SIZE = size_t(log10(SIZE_MAX) / log10(BASE)) + 1;
        public:
            BigInt() = default;
            template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type>
            BigInt(const Tint number);
            BigInt(const BigInt& copied);
            BigInt(BigInt&& moved);
            BigInt& operator=(const BigInt& copied);
            BigInt& operator=(BigInt&& moved);
            ~BigInt() = default;

            BigInt operator-() const;

            void operator-=(const BigInt& other);

            BigInt& operator++();
            BigInt operator++(int);

            BigInt& operator+=(const BigInt& other);
            BigInt operator+(const BigInt& rhs) const &;
            BigInt operator+(const BigInt& rhs) && ;
            BigInt operator+(BigInt&& rhs) const &;
            BigInt operator+(BigInt&& rhs) &&;


            //template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type>
            //const BigInt operator+(const Tint other) const;

            //friend const BigInt operator+(BigInt&& lhs, BigInt&& rhs);
            template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type> // IGNORE VS2017 IntelliSense complains E2611
            friend const BigInt operator+(const Tint lhs, const BigInt& rhs);

            const BigInt& operator--();
            const BigInt operator--(int);
            const BigInt operator-(const BigInt& other) const;
            //template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type>
            //const BigInt operator-(const Tint other) const;
            template<typename Tint, class = typename std::enable_if<std::is_integral<Tint>::value>::type> // IGNORE VS2017 IntelliSense complains E2611
            friend const BigInt operator-(const Tint lhs, const BigInt& rhs);

            const bool operator<(const BigInt& other) const {
                if (is_positive_ != other.is_positive_)
                    return !is_positive_;
                return CompareAbs(other) == (is_positive_ ? -1 : 1);
            }

            const bool operator<=(const BigInt& other) const {
                if (is_positive_ != other.is_positive_)
                    return !is_positive_;
                int compare = CompareAbs(other);
                return compare == 0 || compare == (is_positive_ ? -1 : 1);
            }

            friend std::ostream& operator<<(std::ostream& out, const BigInt& number);
        private:
            bool is_positive_;
            Container<base_t> digits = Container<base_t>(INIT_SIZE);

            bool LessAbs(const BigInt& rhs);
            int CompareAbs(const BigInt& rhs) const;
            void AddAbs(const BigInt& other);
            void SubtractAbs(const BigInt& other, bool is_less_than_other);
        };

#pragma region BigInt

#pragma region constructors
        template<typename Tint, class>
        inline BigInt::BigInt(const Tint number) : is_positive_(number >= 0) {
            size_t module = stl::Abs(number);
            size_t next = 0;
            int sign = stl::Sign(number);
            while (module) {
                digits[next++] = (base_t)(module % BASE);
                module /= BASE;
            }
            digits.size_ = next;
        }

        inline BigInt::BigInt(const BigInt & copied) :
            is_positive_(copied.is_positive_),
            digits(copied.digits) {}

        inline BigInt::BigInt(BigInt&& moved) :
            is_positive_(moved.is_positive_),
            digits(std::move(moved.digits)) {}


        BigInt& BigInt::operator=(const BigInt& copied) {
            if (this == &copied) {
                return *this;
            }
            is_positive_ = copied.is_positive_;
            digits = copied.digits;
            return *this;
        }

        BigInt& BigInt::operator=(BigInt&& moved) {
            if (this == &moved) {
                return *this;
            }
            is_positive_ = moved.is_positive_;
            digits = std::move(moved.digits);
            return *this;
        }
#pragma endregion constructors

        inline BigInt BigInt::operator-() const {
            BigInt result(*this);
            result.is_positive_ = !is_positive_;
            return result;
        }



        void BigInt::operator-=(const BigInt& other) {
            is_positive_ = !is_positive_;
            *this += other;
            is_positive_ = !is_positive_;
        }

#pragma region operator+
        BigInt& BigInt::operator+=(const BigInt& other) {
            if (is_positive_ == other.is_positive_) {
                AddAbs(other);
                return *this;
            }
            bool is_less = (CompareAbs(other) == -1);
            SubtractAbs(other, is_less);
            is_positive_ = (is_positive_ != is_less);
            return *this;
        }

        inline BigInt BigInt::operator+(const BigInt& rhs) const & {
            BigInt result(*this);
            result += rhs;
            return result;
        }

        inline BigInt BigInt::operator+(const BigInt& rhs) && {
            return std::move(*this += rhs);
        }

        inline BigInt BigInt::operator+(BigInt && rhs) const & {
            return std::move(rhs += *this);
        }

        inline BigInt BigInt::operator+(BigInt && rhs) && {
            return std::move(*this += rhs);
        }

        template<typename Tint, class>
        inline const BigInt operator+(const Tint lhs, const BigInt& rhs) {
            return rhs + lhs;
        }
#pragma endregion operator+

#pragma region operator-
        const BigInt& BigInt::operator--() {
            *this -= 1;
            return *this;
        }

        const BigInt BigInt::operator--(int) {
            BigInt result(*this);
            *this -= 1;
            return result;
        }

        const BigInt BigInt::operator-(const BigInt& other) const {
            BigInt result(*this);
            result -= other;
            return result;
        }

        //template<typename Tint, class>
        //const BigInt BigInt::operator-(const Tint other) const {
        //    BigInt result(*this);
        //    result -= BigInt(other);
        //    return result;
        //}

        template<typename Tint, class>
        const BigInt operator-(const Tint lhs, const BigInt& rhs) {
            return -rhs + lhs;
        }
#pragma endregion operator-

#pragma region operator++ --
        BigInt & BigInt::operator++() {
            *this += 1;
            return *this;
        }

        BigInt BigInt::operator++(int) {
            BigInt result(*this);
            *this += 1;
            return result;
        }
#pragma endregion operator++ --

        std::ostream& operator<<(std::ostream& out, const BigInt& number) {
            std::string result;
            if (!number.is_positive_) {
                out << "-";
            }
            out << number.digits;
            return out;
        }

        bool BigInt::LessAbs(const BigInt& rhs) {
            const size_t size = digits.size_;
            if (size < rhs.digits.size_) return true;
            else if (size > rhs.digits.size_) return false;
            base_t* a = digits.buffer_ + size - 1;
            base_t* b = rhs.digits.buffer_ + size - 1;
            for (; a >= digits.buffer_; --a, --b) {
                if (*a < *b) return true;
                else if (*a > *b) return false;
            }
            return false;
        }

        int BigInt::CompareAbs(const BigInt& rhs) const {
            const size_t size = digits.size_;
            if (size < rhs.digits.size_) return -1;
            else if (size > rhs.digits.size_) return 1;
            base_t* a = digits.buffer_ + size - 1;
            base_t* b = rhs.digits.buffer_ + size - 1;
            for (; a >= digits.buffer_; --a, --b) {
                if (*a < *b) return -1;
                else if (*a > *b) return 1;
            }
            return 0;
        }

        void BigInt::AddAbs(const BigInt& other) {
            size_t min_size = std::min(digits.size_, other.digits.size_);
            size_t max_size = std::max(digits.size_, other.digits.size_);
            digits.SetSize(max_size + 1); // set trailing zeros to result number
            // r = a - b; digits(a) >= digits(b);
            base_t *r = digits.buffer_;
            base_t *a = r;
            base_t *b = other.digits.buffer_;
            if (other.digits.size_ != min_size) {
                std::swap(a, b);
            }
            unsigned int carry = 0;
            // iterate over 0 - min_size
            for (size_t i = 0; i < min_size; ++i, ++a, ++b, ++r) {
                base_t t = *a + *b + carry;
                unsigned int temp_carry = BASE <= t;
                *r = t - temp_carry * BASE;
                carry = temp_carry;
            }
            // iterate over min_size - max_size with carry.
            for (size_t i = min_size; (i < max_size); ++i, ++a, ++r) {
                base_t t = *a + carry;
                unsigned int temp_carry = BASE <= t;
                *r = t - temp_carry * BASE;
                carry = temp_carry;
            }
            // TODO 1. stop when carry is 0 
            // 2. memcpy if *a is &other
            *r = carry; // we could overlflow. example: base = 10; 5 + 6 = 11, extra digit in result
            digits.TrimSize();
        }

        void BigInt::SubtractAbs(const BigInt& other, bool is_less_than_other) {
            size_t min_size = std::min(digits.size_, other.digits.size_);
            size_t max_size = std::max(digits.size_, other.digits.size_);
            digits.SetSize(max_size); // set trailing zeros to result number
            // 1. r = a - b; a >= b;
            base_t *r = digits.buffer_;
            base_t *a = r;
            base_t *b = other.digits.buffer_;
            if (is_less_than_other) {
                std::swap(a, b);
            }
            unsigned int carry = 0;
            // 2 iterate over 0 - min_size
            for (size_t i = 0; i < min_size; ++i, ++a, ++b, ++r) {
                base_t t = *b + carry;
                unsigned int temp_carry = *a < t;
                *r = BASE * temp_carry + *a - t;
                carry = temp_carry;
            }
            // 2 iterate over min_size - max_size with carry. example: 100 - 1
            for (size_t i = min_size; (i < digits.size_); ++i, ++a, ++r) {
                unsigned int temp_carry = *a < carry;
                *r = BASE * temp_carry + *a - carry;
                carry = temp_carry;
            }
            // TODO 1. stop when carry is 0 
            // 2. memcpy if *a is &other
            digits.TrimSize();
        }

#pragma endregion BigInt

    }
}

#endif  // !SERIALIZER_H_