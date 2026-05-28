#pragma once
#include <stdexcept>
#include <algorithm>
#include <limits>

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
    // --- Iterator tipai ---
    using iterator       = T*;
    using const_iterator = const T*;

    // --- begin / end ---
    iterator begin()             { return data_; }
    iterator end()               { return data_ + size_; }

    const_iterator begin()  const { return data_; }
    const_iterator end()    const { return data_ + size_; }

    const_iterator cbegin() const { return data_; }
    const_iterator cend()   const { return data_ + size_; }

    // --- Konstruktoriai ---
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    ~Vector() { delete[] data_; }

    // --- Copy konstruktorius ---
    Vector(const Vector& other)
    : data_(new T[other.capacity_]),
      size_(other.size_),
      capacity_(other.capacity_) {
    for (size_t i = 0; i < size_; ++i)
        data_[i] = other.data_[i];
    }

    // --- Move konstruktorius ---
    Vector(Vector&& other) noexcept
    : data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_) {
    other.data_     = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
    }

    // --- Copy operator= ---
    Vector& operator=(const Vector& other) {
    if (this == &other) return *this;
    delete[] data_;
    capacity_ = other.capacity_;
    size_     = other.size_;
    data_     = new T[capacity_];
    for (size_t i = 0; i < size_; ++i)
        data_[i] = other.data_[i];
    return *this;
    }

    // --- Move operator= ---
    Vector& operator=(Vector&& other) noexcept {
    if (this == &other) return *this;
    delete[] data_;
    data_           = other.data_;
    size_           = other.size_;
    capacity_       = other.capacity_;
    other.data_     = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
    return *this;
}

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

    // --- reserve ---
    void reserve(size_t newCap) {
        if (newCap <= capacity_) return;
        reallocate(newCap);
    }

    // --- shrink_to_fit ---
    void shrink_to_fit() {
        if (size_ == capacity_) return;
        if (size_ == 0) {
            delete[] data_;
            data_     = nullptr;
            capacity_ = 0;
            return;
        }
        reallocate(size_);
    }

    // --- max_size ---
    size_t max_size() const {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }
    // --- at (su ribų tikrinimu) ---
    T& at(size_t i) {
        if (i >= size_)
            throw std::out_of_range("Vector::at – indeksas " +
                                    std::to_string(i) +
                                    " >= size " +
                                    std::to_string(size_));
        return data_[i];
    }

    const T& at(size_t i) const {
        if (i >= size_)
            throw std::out_of_range("Vector::at – indeksas " +
                                    std::to_string(i) +
                                    " >= size " +
                                    std::to_string(size_));
        return data_[i];
    }

    // --- front / back ---
    T&       front()       { return data_[0]; }
    const T& front() const { return data_[0]; }

    T&       back()        { return data_[size_ - 1]; }
    const T& back()  const { return data_[size_ - 1]; }

    // --- data (raw pointer) ---
    T*       data()        { return data_; }
    const T* data()  const { return data_; }
};
