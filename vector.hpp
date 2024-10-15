#pragma once
#include <cstdlib>
#include <initializer_list>
#include <memory>

template <typename T>
struct DefaultAllocator {
    T* allocate(size_t amount) const {  // NOLINT
        return new T[amount];
    }

    void deallocate(T* from, size_t amount) const noexcept {  // NOLINT
        delete[] from;
    }
};

template <typename T, typename Allocator = DefaultAllocator<T>>
class Vector {
    template <typename Y, typename OtherAllocator>
    friend class Vector;

public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;  // NOLINT

        Iterator(T* element) : vector_element_(element) {
        }

        Iterator& operator++() {
            ++vector_element_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator result = *this;
            ++vector_element_;
            return result;
        }

        Iterator operator+(ptrdiff_t shift) {
            return Iterator(vector_element_ + shift);
        }

        Iterator operator-(ptrdiff_t shift) {
            return Iterator(vector_element_ - shift);
        }

        T& operator*() const {
            return *vector_element_;
        }

        T* operator->() const {
            return vector_element_;
        }

        bool operator==(const Iterator& rhs) const {
            return vector_element_ == rhs.vector_element_;
        }

        bool operator!=(const Iterator& rhs) const {
            return vector_element_ != rhs.vector_element_;
        }

    private:
        T* vector_element_;
    };

    Vector() : data_(nullptr), size_(0), capacity_(0), allocator_() {
    }

    explicit Vector(size_t n) : allocator_(), size_(n), capacity_(n) {
        data_ = allocator_.allocate(capacity_);
    }

    Vector(size_t n, const T& default_value) : Vector(n) {
        for (size_t index = 0; index < size_; ++index) {
            data_[index] = default_value;
        }
    }

    Vector(std::initializer_list<T> list) : Vector(list.size()) {
        size_t index = 0;
        for (const T& element : list) {
            data_[index] = element;
            ++index;
        }
    }

    Vector(const Vector<T, Allocator>& other) : Vector(other.capacity_) {
        size_ = other.size_;
        for (size_t index = 0; index < size_; ++index) {
            data_[index] = other[index];
        }
    }

    Vector(const Vector<T, Allocator>&& other) : Vector() {
        Swap(other);
    }

    Vector<T, Allocator>& operator=(const Vector<T, Allocator>& other) {
        if (this == &other) {
            return *this;
        }
        Reset();
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = allocator_.allocate(capacity_);
        for (size_t index = 0; index < size_; ++index) {
            data_[index] = other[index];
        }
        return *this;
    }

    Vector<T, Allocator>& operator=(const Vector<T, Allocator>&& other) {
        if (this == &other) {
            return *this;
        }
        Swap(other);
        return *this;
    }

    ~Vector() {
        Reset();
    }

    void Assign(size_t n, const T& value) {
        Reset();
        size_ = capacity_ = n;
        data_ = allocator_.allocate(capacity_);
        for (size_t index = 0; index < n; ++index) {
            data_[index] = value;
        }
    }

    void Swap(Vector<T>& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(allocator_, other.allocator_);
    }

    void PushBack(const T& value) {
        ReallocateIfNeeded();
        data_[size_] = value;
        ++size_;
    }

    void PopBack() {
        --size_;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    T operator[](size_t index) const {
        return data_[index];
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    void Clear() {
        while (size_ > 0) {
            PopBack();
        }
    }

    void Resize(size_t new_size) {
        while (size_ > new_size) {
            PopBack();
        }
        while (size_ < new_size) {
            PushBack(T());
        }
    }

    T& Front() {
        return data_[0];
    }

    const T& Front() const {
        return data_[0];
    }

    T& Back() {
        return data_[size_ - 1];
    }

    const T& Back() const {
        return data_[size_ - 1];
    }

    T* Data() const {
        return data_;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        Reallocate(new_capacity);
    }

    Iterator begin() const {  // NOLINT
        return Iterator{&data_[0]};
    }

    Iterator end() const {  // NOLINT
        return Iterator{&data_[size_]};
    }

    void Insert(Iterator position, const T& value) const {
        ReallocateIfNeeded();
        Iterator it = end();
        while (it != position) {
            Iterator prev = it;
            --prev;
            *it = *prev;
            it = prev;
        }
        *it = value;
    }

    void Erase(Iterator position) {
        --size_;
        while (position != end()) {
            Iterator next_it = position;
            ++next_it;
            *position = *next_it;
            position = next_it;
        }
    }

    void ShrinkToFit() {
        Reallocate(size_);
    }

private:
    void Reallocate(size_t new_capacity) noexcept {
        T* new_data = allocator_.allocate(new_capacity);
        for (size_t index = 0; index < size_; ++index) {
            new_data[index] = data_[index];
        }
        allocator_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void ReallocateIfNeeded() noexcept {
        if (size_ == capacity_) {
            Reallocate(2 * capacity_);
        }
    }

    void Reset() noexcept {
        allocator_.deallocate(data_, capacity_);
        size_ = 0;
        capacity_ = 0;
    }

    T* data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;
};

template <typename T, typename Allocator>
bool operator==(const Vector<T, Allocator>& v1, const Vector<T, Allocator>& v2) {
    if (v1.Size() != v2.Size()) {
        return false;
    }
    for (size_t index = 0; index < v1.Size(); ++index) {
        if (v1[index] != v2[index]) {
            return false;
        }
    }
    return true;
}