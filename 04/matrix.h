#pragma once
#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdexcept>

namespace made {

    namespace math {
        typedef unsigned int uint;

        class Matrix {
            class Row {
                friend class Matrix;
            public:
                int &operator[](uint index) {
                    if (index >= size_) {
                        throw std::out_of_range("");
                    }
                    return row_[index];
                }
            private:
                Row(int *data, uint size) : row_(data), size_(size) {}
                int *row_ = nullptr;
                uint size_ = 0;
            };
            friend class Row;
        public:
            Matrix() = default;
            Matrix(const uint rows, const uint cols) : cols_(cols), rows_(rows), data_(new int[cols*rows]) {}
            ~Matrix() { delete data_; }
            Row operator[](uint index) {
                if (index >= rows_) {
                    throw std::out_of_range("");
                }
                return Row(&data_[index * cols_], cols_);
            }
            const uint rows() { return rows_; }
            const uint cols() { return cols_; }
        private:
            int *data_ = nullptr;
            uint rows_ = 0;
            uint cols_ = 0;
        };


    }
}

#endif  // !MATRIX_H_