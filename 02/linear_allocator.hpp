#pragma once
#ifndef LINEAR_ALLOCATOR_HPP_
#define LINEAR_ALLOCATOR_HPP_
#include <exception>
#include <string>

namespace made {

    namespace stl {

        typedef size_t size_type;

        class OutOfMemory : public std::exception {
        public:
            OutOfMemory() noexcept : std::exception() {}
            const char* what() { return "out of memory"; }
        };

        template <class T>
        class LinearAllocator
        {
        public:
            LinearAllocator(size_type max_size);
            ~LinearAllocator();
            T* Alloc(size_type size);
            void Reset();
        private:
            void* buffer_ = nullptr;
            T* tail_ = nullptr;
            T* right_border_ = nullptr;
            size_type max_size_ = 0;
        };

        template <class T>
        LinearAllocator<T>::LinearAllocator(size_type max_size) : max_size_(max_size) {
            buffer_ = std::malloc(max_size_ * sizeof(T));
            if (!buffer_) {
                throw std::bad_alloc();
            }
            Reset();
            right_border_ = tail_ + max_size_;
        }

        template <class T>
        LinearAllocator<T>::~LinearAllocator() {
            if (buffer_) {
                std::free(buffer_);
            }
        }

        template <class T>
        T* LinearAllocator<T>::Alloc(size_type requested_size) {
            size_type memory_left = right_border_ - tail_;
            if (requested_size > memory_left) {
                throw OutOfMemory();
            }
            T* result = tail_;
            tail_ += requested_size;
            return result;
        }

        template <class T>
        void LinearAllocator<T>::Reset() {
            tail_ = static_cast<T*>(buffer_);
        }
    }

}

#endif  // !LINEAR_ALLOCATOR_HPP_
