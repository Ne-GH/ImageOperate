#include "pch.h"
#include "ImageOperator.h"
#include <opencv2/highgui/highgui_c.h>
#include <Windows.h>
#include <string>
#include "MultArray.hpp"

#include "MainApp.h"

template<typename T>
constexpr size_t nl::ImageOperator::get_elem_size() {
    if constexpr (std::is_same_v<T,BGRPixel>) 
        return 3;
    else if (std::is_same_v<T,uchar>)
        return 1;
}

void show_message_box(const std::string& message) {
    CString msg(message.c_str());
    AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}
nl::ImageOperator::ImageOperator() : nl::ImageOperator("") {  }
nl::ImageOperator::ImageOperator(const std::filesystem::path& path) {
    cv::namedWindow("ImageShowWindow");
    window_handle_ = (HWND)cvGetWindowHandle("ImageShowWindow");
    ShowWindow(window_handle_, 0);
}

nl::ImageOperator::~ImageOperator() {
    cv::destroyWindow("ImageShowWindow");
}

template<typename T>
std::tuple<nl::MultArray<T>,int,int> nl::ImageOperator::get_image_data() {
    nl::MultArray<T> data(reinterpret_cast<T *>(image_.data), { image_.rows, image_.cols });
    return { data, image_.rows, image_.cols };
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
    auto [data, row, col] = get_image_data<BGRPixel>();

    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col / 2; ++j)
            std::swap(data({ i,j }) , data({ i,col - j - 1 }));

    return *this;
}

/*
 * @brief : 竖直翻转
*/
nl::ImageOperator& nl::ImageOperator::reverse_vertically() {

    int row = image_.rows, col = image_.cols;
    uchar* data = image_.data;
    auto buf = new uchar[col * 3];
    
#define LINE(num) ((num) * col * 3)

    for (int i = 0; i < row / 2; ++i) {
        memcpy(buf,                         &data[LINE(i)],             col * 3);
        memcpy(&data[LINE(i)],              &data[LINE(row - i - 1)],   col * 3);
        memcpy(&data[LINE(row - i - 1)],    buf,                        col * 3);
    }

#undef LINE
    delete[] buf;

    return *this;
}

nl::ImageOperator& nl::ImageOperator::to_grayscale() {

    auto [data, row, col] = get_image_data<BGRPixel>();

    auto image = cv::Mat(row, col, CV_8UC1);
    nl::MultArray<uchar> image_data(image.data, { row,col });

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            auto pixel = data({ i,j });
            int gray = pixel.R * 0.299 + pixel.G * 0.587 + pixel.B * 0.114;
            image_data({ i,j }) = gray;
        }
    }

    image_ = image;

    return *this;
}

nl::ImageOperator& nl::ImageOperator::to_binary(int threshold) {
    int elem_size = image_.elemSize();

    // @TODO 未处理RGB图像
    if (elem_size != 1)
        return *this;

    int row = image_.rows, col = image_.cols;
    nl::MultArray<uchar> data(image_.data, { row, col });


    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (data({ i, j }) > threshold) 
                data({ i, j }) = 255;
            else
                data({ i, j }) = 0;
        }
    }
    return *this;
}

nl::ImageOperator& nl::ImageOperator::to_pseudo_color() {

    return *this;
}

std::vector<std::array<size_t, 256>> nl::ImageOperator::get_histogram_data() {
    size_t pixel_size = image_.elemSize();

    // 灰度图
    if (pixel_size == 1) {
        std::array<size_t, 256> count = { 0 };
        auto [data, row, col] = get_image_data<uchar>();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                count[data({ i, j })]++;
            }
        }
        return { count };
    }
    // 彩色图 imread 读取类型为 BGR
    else if (pixel_size == 3) {
        std::vector<std::array<size_t, 256>> count(3);
        auto [data, row, col] = get_image_data<BGRPixel>();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                (count[B])[data({ i,j }).B]++;
                (count[G])[data({ i,j }).G]++;
                (count[R])[data({ i,j }).R]++;
            }
        }
        return count;
    }
	else {
        throw std::exception("no support image type");
	}

}

HWND nl::ImageOperator::get_show_window() {
    return window_handle_;
}

void nl::ImageOperator::show_image() {
    cv::imshow("ImageShowWindow", image_);
}

