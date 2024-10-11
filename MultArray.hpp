#pragma once

#include <vector>
#include <span>

#define NAMESPACE_BEGIN(name) namespace name {
#define NAMESPACE_END(name) }

NAMESPACE_BEGIN(nl)


template<typename T>
class MultArray {
    T* data_;
    size_t count_;
    std::vector<int> dimensions_;
public:
    MultArray(T* p, std::vector<int> dims) : data_(p), dimensions_(dims) { 
        count_ = 1;
        for (int val : dimensions_)
            count_ *= val;
    }

    T& operator()(std::vector<int> indices) {
        if (indices.size() != dimensions_.size()) 
            throw std::invalid_argument("Number of indices must match the number of dimensions.");

        auto index = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] < 0 || indices[i] >= dimensions_[i]) {
                throw std::out_of_range("Index out of range.");
            }
            index = index * dimensions_[i] + indices[i];
        }
        return data_[index];
    }

    size_t size() {
        return dimensions_[0];
    }

    MultArray<T> operator[](size_t index) {
        if (dimensions_.size() <= 1)
            throw std::out_of_range("dimension less one");

        return MultArray<T>(data_ + index * count_ / dimensions_[0],std::vector(dimensions_.begin() + 1,dimensions_.end()));
    }

    std::span<T> to_span() {
        return std::span<T>(data_, count_);
    }

    void swap(std::span<T> sp1, std::span<T> sp2, T buf[], size_t elem_size) {
        memcpy(buf,         sp1.data(),     sp1.size() * elem_size);
        memcpy(sp1.data(),  sp2.data(),     sp1.size() * elem_size);
        memcpy(sp2.data(),  buf,            sp1.size() * elem_size);
    }

};

NAMESPACE_END(nl)
