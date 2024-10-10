#include "pch.h"
#include "ImageOperator.h"
#include <opencv2/highgui/highgui_c.h>
#include <Windows.h>
#include "MultArray.hpp"

#include "MainApp.h"

nl::ImageOperator::ImageOperator() : nl::ImageOperator("") {  }
nl::ImageOperator::ImageOperator(const std::filesystem::path& path) {
    cv::namedWindow("ImageShowWindow");
    window_handle_ = (HWND)cvGetWindowHandle("ImageShowWindow");
    ShowWindow(window_handle_, 0);
}

nl::ImageOperator::~ImageOperator() {
    cv::destroyWindow("ImageShowWindow");
}


void nl::ImageOperator::open(const std::filesystem::path& path) {
    if (!path.empty() && std::filesystem::exists(path)) {
        image_ = cv::imread(path.generic_string());
    }
    show_image();
}

void nl::ImageOperator::save() {

}

nl::ImageOperator& nl::ImageOperator::zoom(int multiple) {

    return *this;
}
nl::ImageOperator& nl::ImageOperator::resize(int width, int height) {

    return *this;

}

nl::ImageOperator& nl::ImageOperator::rotation(int angle) {

    return *this;
}

nl::ImageOperator& nl::ImageOperator::rotation(int angle,int x,int y) {

    return *this;
}

/*
 * @brief : 水平翻转
*/
nl::ImageOperator& nl::ImageOperator::reverse_horizontally() {
    int row = image_.rows;
    int col = image_.cols;

    nl::MultArray<RGBPixel> data(reinterpret_cast<RGBPixel*>(image_.data), { row, col });

    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col / 2; ++j)
            std::swap(data({ i,j }) , data({ i,col - j - 1 }));


    return *this;
}

/*
 * @brief : 竖直翻转
*/
nl::ImageOperator& nl::ImageOperator::reverse_vertically() {
    int row = image_.rows;
    int col = image_.cols;

    nl::MultArray<RGBPixel> data(reinterpret_cast<RGBPixel*>(image_.data), { row, col });

    for (int i = 0; i < row / 2; ++i)
        for (int j = 0; j < col; ++j)
            std::swap(data({ i, j }), data({ row - i - 1, j}));

    return *this;
}
nl::ImageOperator& nl::ImageOperator::to_grayscale() {

    return *this;
}

nl::ImageOperator& nl::ImageOperator::to_binary() {

    return *this;
}

nl::ImageOperator& nl::ImageOperator::to_pseudo_color() {

    return *this;
}

std::array<size_t, 256> nl::ImageOperator::get_histogram_data() {

    std::array<size_t, 256> count = { 0 };
    int row = image_.rows, col = image_.cols, size = image_.elemSize();

    // @TODO 暂时仅实现灰度图的直方图
    if (size != 1)
        return {};

    void* data = image_.data;









    return {};
}

HWND nl::ImageOperator::get_show_window() {
    return window_handle_;
}

void nl::ImageOperator::show_image() {
    cv::imshow("ImageShowWindow", image_);
}

