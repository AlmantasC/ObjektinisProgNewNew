#pragma once
#include <stdexcept>
#include <algorithm>

template <typename T>
class Vector {
private:
    T*     data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t newCap) {
        T* newData = new T[newCap];
        for (size_t i = 0; i < size_; ++i)
            newData[i] = std::move(data_[i]);
        delete[] data_;
        data_     = newData;
        capacity_ = newCap;
    }

public:
    // --- Konstruktoriai ---
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    ~Vector() { delete[] data_; }

    // --- Capacity ---
    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
    bool   empty()    const { return size_ == 0; }

    // --- Modifier ---
    void push_back(const T& val) {
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = val;
    }

    // --- Element access ---
    T& operator[](size_t i)             { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
};
