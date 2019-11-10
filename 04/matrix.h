#pragma once
#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdexcept>

namespace made {

    namespace math {
        typedef unsigned int uint;

        class Matrix {
        public:
            Matrix() = default;
            Matrix(const uint rows, const uint cols): cols_(cols), rows_(rows), data_(new int[cols*rows]) {}
            ~Matrix() { delete data_; }
            int &operator[](uint index) {
                if (index >= cols_) {
                    throw std::out_of_range("");
                }
                return data_[index];
            }
            const uint rows() { return rows_; }
            const uint cols() { return cols_; }
        private:
            int *data_ = nullptr;
            uint rows_ = 0;
            uint cols_ = 0;
            class Row {
            private:
                int *row_ = nullptr;
            };
        };


    }
}

#endif  // !MATRIX_H_