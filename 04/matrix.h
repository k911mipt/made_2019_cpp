#pragma once
#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdexcept>
#include <cstring>

namespace made {

    namespace math {
        typedef unsigned int uint;

        class Matrix {
        private:
            class Row;
        public:
            Matrix() = default;
            Matrix(const uint rows, const uint cols) : cols_(cols), rows_(rows), size_(cols*rows) {//, data_(new int[size_]) {
                data_ = new int[size_];
            }
            ~Matrix();
            Row operator[](uint index);
            const uint rows() { return rows_; }
            const uint cols() { return cols_; }
            void Clear();
            Matrix& operator*=(const int multiplier);
            //friend bool operator==(const Matrix& lhs, const  Matrix& rhs);
            bool operator==(const  Matrix& rhs);
            bool operator!=(const  Matrix& rhs) { return !(*this == rhs); }
            void Print();
        private:
            int *data_ = nullptr;
            uint rows_ = 0;
            uint cols_ = 0;
            uint size_ = 0;
        };

        Matrix::~Matrix() {
            delete[]data_;
            data_ = nullptr;
        }

        class Matrix::Row {
        public:
            Row(int *data, uint size) : row_(data), size_(size) {}
            int &operator[](uint index);
        private:
            int *row_ = nullptr;
            uint size_ = 0;
        };

#pragma region Matrix
        void Matrix::Clear() {
            if (data_) {
                memset(data_, 0, sizeof(int) * size_);
            }
        }

        Matrix& Matrix::operator*=(const int multiplier) {
            for (int *p = data_, *end = data_ + size_; p < end; ++p) {
                *p *= multiplier;
            }
            return *this;
        }

        inline bool Matrix::operator==(const Matrix & rhs) {
            bool result = (size_ == rhs.size_);
            if (result)
                result = (rows_ == rhs.rows_);
            if (result) {
                //result = memcmp(data_, rhs.data_, size_ * sizeof(uint)); // <-- This lies. https://en.cppreference.com/w/cpp/string/byte/memcmp
                for (uint i = 0; i < rows_; ++i) {
                    for (uint j = 0; j < cols_; ++j) {
                        if (data_[i * cols_ + j] != rhs.data_[i * cols_ + j]) {
                            return false;
                        }
                    }
                }
            }
            return result;
        }

        Matrix::Row Matrix::operator[](uint index) {
            if (index >= rows_) {
                throw std::out_of_range("");
            }
            return Row(&data_[index * cols_], cols_);
        }

        void Matrix::Print() {
            std::cout << std::endl;
            for (uint i = 0; i < rows_; ++i) {
                for (uint j = 0; j < cols_; ++j) {
                    std::cout << data_[i * cols_ + j] << " ";
                }
                std::cout << std::endl;
            }
        }
#pragma endregion

#pragma region Row
        int& Matrix::Row::operator[](uint index) {
            if (index >= size_) {
                throw std::out_of_range("");
            }
            return row_[index];
        }
#pragma endregion

    }
}

#endif  // !MATRIX_H_