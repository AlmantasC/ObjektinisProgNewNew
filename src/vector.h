#pragma once
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <initializer_list>

// =============================================================
// commit #1: skeletas – data_, size_, capacity_, push_back, []
// =============================================================

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
    // --- Destruktorius ---
    ~Vector() { delete[] data_; }

    // --- Capacity ---
    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
    bool   empty()    const { return size_ == 0; }

    // --- push_back ---
    void push_back(const T& val) {
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = val;
    }

    // --- operator[] ---
    T&       operator[](size_t i)       { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    // =============================================================
    // commit #2: copy/move konstruktoriai ir operator=
    // =============================================================

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    Vector(size_t n, const T& val = T())
        : data_(new T[n]), size_(n), capacity_(n) {
        for (size_t i = 0; i < n; ++i)
            data_[i] = val;
    }

    Vector(std::initializer_list<T> il)
        : data_(new T[il.size()]), size_(il.size()), capacity_(il.size()) {
        size_t i = 0;
        for (const auto& v : il)
            data_[i++] = v;
    }

    Vector(const Vector& other)
        : data_(new T[other.capacity_]),
          size_(other.size_),
          capacity_(other.capacity_) {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_) {
        other.data_     = nullptr;
        other.size_     = 0;
        other.capacity_ = 0;
    }

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

    Vector& operator=(std::initializer_list<T> il) {
        delete[] data_;
        size_     = il.size();
        capacity_ = il.size();
        data_     = new T[capacity_];
        size_t i  = 0;
        for (const auto& v : il)
            data_[i++] = v;
        return *this;
    }

    // =============================================================
    // commit #3: iteratoriai – begin, end, cbegin, cend
    // =============================================================

    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin()              { return data_; }
    iterator end()                { return data_ + size_; }
    const_iterator begin()  const { return data_; }
    const_iterator end()    const { return data_ + size_; }
    const_iterator cbegin() const { return data_; }
    const_iterator cend()   const { return data_ + size_; }

    reverse_iterator rbegin()              { return reverse_iterator(end()); }
    reverse_iterator rend()                { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend()   const { return const_reverse_iterator(cbegin()); }

    // =============================================================
    // commit #4: capacity funkcijos – reserve, shrink_to_fit
    // =============================================================

    void reserve(size_t newCap) {
        if (newCap <= capacity_) return;
        reallocate(newCap);
    }

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

    size_t max_size() const {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    // =============================================================
    // commit #5: element access – at, front, back, data
    // =============================================================

    T& at(size_t i) {
        if (i >= size_)
            throw std::out_of_range("Vector::at – indeksas " +
                                    std::to_string(i) + " >= size " +
                                    std::to_string(size_));
        return data_[i];
    }

    const T& at(size_t i) const {
        if (i >= size_)
            throw std::out_of_range("Vector::at – indeksas " +
                                    std::to_string(i) + " >= size " +
                                    std::to_string(size_));
        return data_[i];
    }

    T&       front()       { return data_[0]; }
    const T& front() const { return data_[0]; }

    T&       back()        { return data_[size_ - 1]; }
    const T& back()  const { return data_[size_ - 1]; }

    T*       data()        { return data_; }
    const T* data()  const { return data_; }

    // =============================================================
    // commit #6: modifiers I – insert, erase, clear
    // =============================================================

    void clear() {
        size_ = 0;
    }

    void pop_back() {
        if (size_ > 0) --size_;
    }

    iterator insert(const_iterator pos, const T& val) {
        size_t idx = pos - data_;
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        for (size_t i = size_; i > idx; --i)
            data_[i] = std::move(data_[i - 1]);
        data_[idx] = val;
        ++size_;
        return data_ + idx;
    }

    iterator insert(const_iterator pos, size_t count, const T& val) {
        size_t idx = pos - data_;
        if (size_ + count > capacity_)
            reallocate(std::max(size_ + count, capacity_ * 2));
        for (size_t i = size_ + count - 1; i >= idx + count; --i)
            data_[i] = std::move(data_[i - count]);
        for (size_t i = 0; i < count; ++i)
            data_[idx + i] = val;
        size_ += count;
        return data_ + idx;
    }

    iterator erase(const_iterator pos) {
        size_t idx = pos - data_;
        for (size_t i = idx; i < size_ - 1; ++i)
            data_[i] = std::move(data_[i + 1]);
        --size_;
        return data_ + idx;
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_t idxFirst = first - data_;
        size_t idxLast  = last  - data_;
        size_t count    = idxLast - idxFirst;
        for (size_t i = idxFirst; i < size_ - count; ++i)
            data_[i] = std::move(data_[i + count]);
        size_ -= count;
        return data_ + idxFirst;
    }

    // =============================================================
    // commit #7: modifiers II – resize, emplace_back, assign, swap
    // =============================================================

    void resize(size_t newSize, const T& val = T()) {
        if (newSize > capacity_)
            reallocate(newSize);
        if (newSize > size_)
            for (size_t i = size_; i < newSize; ++i)
                data_[i] = val;
        size_ = newSize;
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_t idx = pos - data_;
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        for (size_t i = size_; i > idx; --i)
            data_[i] = std::move(data_[i - 1]);
        data_[idx] = T(std::forward<Args>(args)...);
        ++size_;
        return data_ + idx;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = T(std::forward<Args>(args)...);
    }

    void push_back(T&& val) {
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = std::move(val);
    }

    void assign(size_t count, const T& val) {
        clear();
        resize(count, val);
    }

    void assign(std::initializer_list<T> il) {
        *this = il;
    }

    void swap(Vector& other) noexcept {
        std::swap(data_,     other.data_);
        std::swap(size_,     other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // =============================================================
    // commit #8: palyginimo operatoriai – ==, !=, <, <=, >, >=
    // =============================================================

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i)
            if (data_[i] != other.data_[i]) return false;
        return true;
    }

    bool operator!=(const Vector& other) const { return !(*this == other); }

    bool operator<(const Vector& other) const {
        return std::lexicographical_compare(begin(), end(),
                                            other.begin(), other.end());
    }

    bool operator<=(const Vector& other) const { return !(other < *this); }
    bool operator>(const Vector& other)  const { return other < *this; }
    bool operator>=(const Vector& other) const { return !(*this < other); }
};

// =============================================================
// commit #8 (tęsinys): ne-narys swap
// =============================================================

template <typename T>
void swap(Vector<T>& a, Vector<T>& b) noexcept {
    a.swap(b);
}
