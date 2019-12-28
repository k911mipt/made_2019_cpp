#pragma once
#ifndef VECTOR_H_
#define VECTOR_H_

#include <cassert>
#include <algorithm>

#include "common.h"

_MADE_BEGIN
_STL_BEGIN

template <class T, class Alloc = std::allocator<T>>
class Vector;

template <class TIterTraits>
class VectorConstIterator {
private:
    template<class Ty, class TAlloc>
    friend class Vector;

    using iter_traits = std::iterator_traits<TIterTraits>;
public:
    using iterator_category = typename TIterTraits::iterator_category;
    using value_type = typename TIterTraits::value_type;
    using difference_type = typename TIterTraits::difference_type;
    using pointer = typename TIterTraits::pointer;
    using reference = typename TIterTraits::reference;
private:
    pointer ptr_;
public:
    explicit VectorConstIterator(pointer ptr) : ptr_(ptr) {}

    [[nodiscard]] reference operator*() const { return *ptr_; }
    [[nodiscard]] const pointer operator->() const { return ptr_; }
    [[nodiscard]] reference operator[](const difference_type offset) const { return *(*this + offset); }
    [[nodiscard]] bool operator==(const VectorConstIterator& rhs) const { return ptr_ == rhs.ptr_; }
    [[nodiscard]] bool operator!=(const VectorConstIterator& rhs) const { return !(*this == rhs); }
    [[nodiscard]] bool operator<(const VectorConstIterator& rhs) const { return ptr_ < rhs.ptr_; }
    [[nodiscard]] bool operator>(const VectorConstIterator& rhs) const { return rhs < *this; }
    [[nodiscard]] bool operator<=(const VectorConstIterator& rhs) const { return !(rhs < *this); }
    [[nodiscard]] bool operator>=(const VectorConstIterator& rhs) const { return !(*this < rhs); }
    [[nodiscard]] difference_type operator-(const VectorConstIterator& rhs) const { return ptr_ - rhs.ptr_; }
    VectorConstIterator& operator++() {
        ++ptr_;
        return *this;
    }
    VectorConstIterator operator++(int) {
        VectorConstIterator temp = *this;
        ++* this;
        return temp;
    }
    VectorConstIterator& operator--() {
        --ptr_;
        return *this;
    }
    VectorConstIterator operator--(int) {
        VectorConstIterator temp = *this;
        --* this;
        return temp;
    }
    VectorConstIterator& operator+=(const difference_type _Off) {
        ptr_ += _Off;
        return *this;
    }
    [[nodiscard]] VectorConstIterator operator+(const difference_type offset) const {
        VectorConstIterator temp = *this;
        return temp += offset;
    }
    VectorConstIterator& operator-=(const difference_type offset) {
        return *this += -offset;
    }
    [[nodiscard]] VectorConstIterator operator-(const difference_type offset) const {
        VectorConstIterator temp = *this;
        return temp -= offset;
    }
};

template <class TIterTraits>
class VectorIterator : public VectorConstIterator<TIterTraits> {
    using Super = VectorConstIterator<TIterTraits>;
public:
    using iterator_category = typename Super::iterator_category;
    using value_type = typename Super::value_type;
    using difference_type = typename Super::difference_type;
    using pointer = typename Super::pointer;
    using reference = typename Super::reference;

    explicit VectorIterator(pointer ptr) : Super(ptr) {}
    [[nodiscard]] reference operator*() const { return const_cast<reference>(Super::operator*()); }
    [[nodiscard]] pointer operator->() const { return const_cast<pointer>(Super::operator->()); }
    [[nodiscard]] reference operator[](const difference_type offset) const { return const_cast<reference>(Super::operator[](offset)); }
    VectorIterator& operator++() {
        Super::operator++();
        return *this;
    }
    VectorIterator operator++(int) {
        VectorIterator temp = *this;
        Super::operator++();
        return temp;
    }
    VectorIterator& operator--() {
        Super::operator--();
        return *this;
    }
    VectorIterator operator--(int) {
        VectorIterator temp = *this;
        Super::operator--();
        return temp;
    }
    VectorIterator& operator+=(const difference_type offset) {
        Super::operator+=(offset);
        return *this;
    }
    [[nodiscard]] VectorIterator operator+(const difference_type offset) const {
        VectorIterator temp = *this;
        return temp += offset;
    }
    VectorIterator& operator-=(const difference_type offset) {
        Super::operator-=(offset);
        return *this;
    }
    using Super::operator-;
    [[nodiscard]] VectorIterator operator-(const difference_type offset) const {
        VectorIterator temp = *this;
        return temp -= offset;
    }
};

template <class T, class TDifference, class TPointer, class TReference>
struct VectorIteratorTraits {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = TDifference;
    using pointer = TPointer;
    using reference = TReference;
};

template <class T, class Alloc>
class Vector {
private:
    using alloc_traits = std::allocator_traits<Alloc>;
public:
    using value_type = T;
    using allocator_type = Alloc;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename alloc_traits::size_type;
    using difference_type = typename alloc_traits::difference_type;
private:
    using iter_traits = VectorIteratorTraits<value_type, difference_type, pointer, reference>;
public:
    using iterator = VectorIterator<iter_traits>;
    using const_iterator = VectorConstIterator<iter_traits>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    allocator_type alloc_;
    size_type capacity_;
    pointer begin_;
    pointer end_;
public:
    Vector()
        : alloc_(allocator_type()),
        capacity_(0),
        begin_(alloc_.allocate(capacity_)),
        end_(begin_)
    {
    }

    explicit Vector(size_type count)
        : alloc_(Alloc()),
        capacity_(std::max(count, static_cast<size_type>(1))),
        begin_(alloc_.allocate(capacity_)),
        end_(begin_)
    {
        for (size_type i = 0; i < capacity_; ++i, ++end_)
            alloc_traits::construct(alloc_, end_);
    }

    Vector(size_type count, const value_type& init_value)
        : alloc_(Alloc()),
        capacity_(std::max(count, static_cast<size_type>(1))),
        begin_(alloc_.allocate(capacity_)),
        end_(begin_)
    {
        for (size_type i = 0; i < capacity_; ++i, ++end_)
            alloc_traits::construct(alloc_, end_, init_value);
    }

    Vector(std::initializer_list<value_type> init_list)
        : alloc_(Alloc()),
        capacity_(std::max(init_list.size(), static_cast<size_type>(1))),
        begin_(alloc_.allocate(capacity_)),
        end_(begin_)
    {
        for (size_type i = 0; i < capacity_; ++i, ++end_)
            alloc_traits::construct(alloc_, end_, *(init_list.begin() + i));
    }

    [[nodiscard]] iterator begin() noexcept { return iterator(begin_); }
    [[nodiscard]] reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(begin_); }
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    [[nodiscard]] iterator end() noexcept { return iterator(end_); }
    [[nodiscard]] reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator(end_); }
    [[nodiscard]] const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    [[nodiscard]] bool empty() const noexcept { return end_ == begin_; }
    [[nodiscard]] size_type size() const noexcept { return end_ - begin_; }
    [[nodiscard]] size_type max_size() const noexcept {
        return std::min(
            static_cast<size_type>((std::numeric_limits<difference_type>::max)()),
            alloc_traits::max_size(alloc_)
        );
    }
    [[nodiscard]] size_type capacity() const noexcept { return capacity_; }
    [[nodiscard]] reference operator[](size_type i) noexcept { return begin_[i]; }
    [[nodiscard]] const_reference operator[](size_type i) const noexcept { return begin_[i]; }
    [[nodiscard]] reference at(size_type pos)  {
        if (size() <= pos)
            ThrowRangeError();
        return begin_[pos]; 
    }
    [[nodiscard]] const_reference at(size_type pos) const  { 
        if (size() <= pos)
            ThrowRangeError();
        return begin_[pos];
    }
    [[nodiscard]] reference front() noexcept { return *begin_; }
    [[nodiscard]] const_reference front() const noexcept { return *begin_; }
    [[nodiscard]] reference back() noexcept { return end_[-1]; }
    [[nodiscard]] const_reference back() const noexcept { return end_[-1]; }

    void push_back(value_type&& value) {
        emplace_back(std::move(value));
    }

    void push_back(const_reference value) {
        emplace_back(value);
    }

    template <class... VT>
    reference emplace_back(VT&&... values) {
        if (size() != capacity_) {
            return EmplaceBackWithUnusedCapacity(std::forward<VT>(values)...);
        }
        reference result = *EmplaceReallocate(end_, std::forward<VT>(values)...);
        return result;
    }

    iterator insert(const_iterator where, T&& value) { return emplace(where, std::move(value)); }
    iterator insert(const_iterator where, const T& value) { return emplace(where, value); }

    template <class... VT>
    iterator emplace(const_iterator where, VT&&... values) {
        const pointer where_ptr = where.ptr_;
        const pointer old_end = end_;
        if (size() != capacity_) {
            if (where_ptr == old_end) {
                EmplaceBackWithUnusedCapacity(std::forward<VT>(values)...);
            }
            else {
                T temp_obj(std::forward<VT>(values)...);
                alloc_traits::construct(alloc_, old_end, std::move(old_end[-1]));
                ++end_;
                std::move_backward(where_ptr, old_end - 1, old_end);
                *where_ptr = std::move(temp_obj);
            }
            return iterator(where_ptr);
        }
        return iterator(EmplaceReallocate(where_ptr, std::forward<VT>(values)...));
    }

    void reserve(size_type count) {
        if (count > capacity()) {
            if (count > max_size()) {
                ThrowLengthError();
            }
            ReallocateExactly(count);
        }
    }

    void resize(size_type newSize) {
        T value; // won't compile if no default constructor. Same for std::vector
        resize(newSize, value);
    }

    void resize(const size_type newsize, const_reference default_value) {
        const auto old_size = size();
        if (newsize < old_size) {
            const pointer new_end = begin_ + newsize;
            Destroy(new_end, end_);
            end_ = new_end;
            return;
        }
        if (newsize > old_size) {
            const auto old_capacity = capacity_;
            if (newsize > old_capacity) {
                ResizeReallocate(newsize, default_value);
                return;
            }
            const pointer old_end = end_;
            end_ = std::uninitialized_fill_n(old_end, newsize - old_size, default_value);
        }
    }

    void pop_back() noexcept {
        alloc_traits::destroy(alloc_, --end_);
    }

    iterator erase(const_iterator where) {
        const pointer where_ptr = where.ptr_;
        std::move(where_ptr + 1, end_, where_ptr);
        alloc_traits::destroy(alloc_, end_ - 1);
        --end_;
        return iterator(where_ptr);
    }

    iterator erase(const_iterator from, const_iterator to) {
        const pointer from_ptr = from.ptr_;
        const pointer new_end = std::move(to.ptr_, end_, from_ptr);
        Destroy(new_end, end_);
        end_ = new_end;
        return iterator(from_ptr);
    }

    void clear() noexcept {
        Destroy(begin_, end_);
        end_ = begin_;
    }

private:
    [[noreturn]] static void ThrowLengthError() {
        throw std::length_error("vector is at max length");
    }

    [[noreturn]] static void ThrowRangeError() {
        throw std::length_error("vector subscript is out of range");
    }

    template <class... VT>
    reference EmplaceBackWithUnusedCapacity(VT&&... values) {
        assert(size() != capacity_);
        alloc_traits::construct(alloc_, end_, std::forward<VT>(values)...);
        reference result = *end_;
        ++end_;
        return result;
    }

    template <class... VT>
    pointer EmplaceReallocate(const pointer where_ptr, VT&&... values) {
        assert(size() == capacity_);
        const size_type where_offset = where_ptr - begin_;
        const size_type old_size = size();
        if (max_size() == old_size)
            ThrowLengthError();

        const size_type new_size = old_size + 1;
        const size_type new_capacity = CalculateGrowth(new_size);

        const pointer new_begin = alloc_.allocate(new_capacity);
        const pointer constructed_last = new_begin + where_offset + 1;
        pointer constructed_first = constructed_last;

        try {
            alloc_traits::construct(alloc_, new_begin + where_offset, std::forward<VT>(values)...);
            constructed_first = new_begin + where_offset;
            if (where_ptr == end_) { // strong guarantee
                std::uninitialized_move(begin_, end_, new_begin);
            }
            else {
                std::uninitialized_move(begin_, where_ptr, new_begin);
                constructed_first = new_begin;
                std::uninitialized_move(where_ptr, end_, new_begin + where_offset + 1);
            }
        }
        catch (...) {
            Destroy(constructed_first, constructed_last);
            alloc_.deallocate(new_begin, new_capacity);
            throw;
        }
        SwapDestroying(new_begin, new_size, new_capacity);
        return new_begin + where_offset;
    }

    size_type CalculateGrowth(const size_type new_size) const {
        const size_type old_capacity = capacity_;
        if (old_capacity > max_size() - old_capacity / 2) {
            return new_size;
        }
        const size_type geometric = old_capacity + old_capacity / 2;
        if (geometric < new_size) {
            return new_size;
        }
        return geometric;
    }

    void ReallocateExactly(const size_type new_capacity) {
        const pointer new_begin = alloc_.allocate(new_capacity);
        try {
            std::uninitialized_move(begin_, end_, new_begin);
        }
        catch (...) {
            alloc_.deallocate(new_begin, new_capacity);
            throw;
        }
        SwapDestroying(new_begin, size(), new_capacity);
    }

    void Destroy(pointer _First, pointer _Last) {
        for (; _First != _Last; ++_First) {
            alloc_traits::destroy(alloc_, _First);
        }
    }

    void SwapDestroying(const pointer new_begin, const size_type new_size, const size_type new_capacity) {
        if (begin_) {
            Destroy(begin_, end_);
            alloc_.deallocate(begin_, capacity_);
        }
        begin_ = new_begin;
        end_ = new_begin + new_size;
        capacity_ = new_capacity;
    }

    void ResizeReallocate(const size_type new_size, const_reference value) {
        if (new_size > max_size()) {
            ThrowLengthError();
        }

        const auto old_size = size();
        const size_type new_capacity = CalculateGrowth(new_size);

        const pointer new_begin = alloc_.allocate(new_capacity);
        const pointer appended_first = new_begin + old_size;
        pointer appended_last = appended_first;

        try {
            appended_last = std::uninitialized_fill_n(appended_first, new_size - old_size, value);
            std::uninitialized_move(begin_, end_, new_begin);
        }
        catch (...) {
            Destroy(appended_first, appended_last);
            alloc_.deallocate(new_begin, new_capacity);
            throw;
        }
        SwapDestroying(new_begin, new_size, new_capacity);
    }

};

_STL_END
_MADE_END

#endif // !VECTOR_H_