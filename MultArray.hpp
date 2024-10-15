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
        auto index = 0;
        for (size_t i = 0; i < indices.size(); ++i) 
            index = index * dimensions_[i] + indices[i];
        return data_[index];
    }

    size_t size() {
        return dimensions_[0];
    }

    MultArray<T> operator[](size_t index) {
        return MultArray<T>(data_ + index * count_ / dimensions_[0],std::vector(dimensions_.begin() + 1,dimensions_.end()));
    }

    std::span<T> to_span() {
        return std::span<T>(data_, count_);
    }

};

NAMESPACE_END(nl)
