#pragma once

#include <vector>
#include <span>

#define NAMESPACE_BEGIN(name) namespace name {
#define NAMESPACE_END(name) }

NAMESPACE_BEGIN(nl)


template<typename T>
class MultArray {
    T* data_;
    std::vector<int> dimensions_;
public:
    MultArray(T* p, std::vector<int> dims) : data_(p), dimensions_(dims) {  }

    T& operator()(std::vector<int> indices) {
        if (indices.size() != dimensions_.size()) 
            throw std::invalid_argument("Number of indices must match the number of dimensions.");

        size_t index = 0;
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

        int begin_index = dimensions_[0];
        for (int i = 1; i < dimensions_.size(); ++i)
            begin_index *= dimensions_[i];

        return MultArray<T>(data_ + begin_index,std::vector(dimensions_.begin() + 1,dimensions_.end()));
    }

    std::span<T> to_span() {
        int count = 1;
        for (auto val : dimensions_)
            count *= val;
        return std::span<T>(data_, count);
    }

};

NAMESPACE_END(nl)
