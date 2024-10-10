#include "pch.h"
#include "ImageOperator.h"
#include <opencv2/highgui/highgui_c.h>
#include <Windows.h>

#include "MainApp.h"

nl::ImageOperator::ImageOperator() : nl::ImageOperator("") {  }
nl::ImageOperator::ImageOperator(const std::filesystem::path& path) {
    cv::namedWindow("ImageShowWindow");
    window_handle_ = (HWND)cvGetWindowHandle("ImageShowWindow");
    // open(path);
}

// @TODO
//nl::ImageOperator::ImageOperator(uchar image[], size_t row, size_t column) {
//	window_handle_ = 0;
//}

void nl::ImageOperator::open(const std::filesystem::path& path) {
    //if (!path.empty() && std::filesystem::exists(path)) {
    //    image_ = cv::imread(path.generic_string());
    //}
}

void nl::ImageOperator::save() {

}

nl::ImageOperator& nl::ImageOperator::zoom(int multiple) {

    return *this;
}

nl::ImageOperator& nl::ImageOperator::rotation(int angle) {

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

std::array<size_t, 256> nl::ImageOperator::get_histogram() {

    return {};
}

HWND nl::ImageOperator::get_show_window() {
    return window_handle_;
}

void nl::ImageOperator::show_image() {
    cv::imshow("ImageShow", image_);//opencv显示图片

}

