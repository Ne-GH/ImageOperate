#pragma once

#include <vector>

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
        if (indices.size() != dimensions_.size()) {
            throw std::invalid_argument("Number of indices must match the number of dimensions.");
        }

        size_t index = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] < 0 || indices[i] >= dimensions_[i]) {
                throw std::out_of_range("Index out of range.");
            }
            index = index * dimensions_[i] + indices[i];
        }
        return data_[index];
    }

};

NAMESPACE_END(nl)
