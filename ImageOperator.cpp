#include "pch.h"
#include "ImageOperator.h"
#include <opencv2/highgui/highgui_c.h>
#include <Windows.h>
#include <string>
#include <tuple>
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
std::tuple<nl::MultArray<T>,int,int> GetImageData(const cv::Mat &image) {
    nl::MultArray<T> data(reinterpret_cast<T *>(image.data), { image.rows, image.cols });
    return { data, image.rows, image.cols };
}

void nl::ImageOperator::open(const std::filesystem::path& path) {
    if (!path.empty() && std::filesystem::exists(path)) {
        image_ = cv::imread(path.generic_string());
        image_back_ = image_;
    }
    show_image();
}

void nl::ImageOperator::save() {

}



nl::ImageOperator& nl::ImageOperator::zoom(double multiple) {

    if (image_.empty() || multiple == 1)
        return *this;

    auto func = [&]<typename T>() {
        int new_width = image_.cols * multiple, new_height = image_.rows * multiple;
        auto new_image = cv::Mat(new_height, new_width, image_.type());
        auto [src_data, src_row, src_col] = GetImageData<T>(image_);
        auto [new_data, ignore1, ignore2] = GetImageData<T>(new_image);

        for (int i = 0; i < new_height; ++i)
            for (int j = 0; j < new_width; ++j)
                new_data({ i,j }) = src_data({ static_cast<int>(i / multiple),static_cast<int>(j / multiple) });

        image_ = new_image;
    };

    // @TODO 二者操作都应该在image_back_ 原图像上完成
    // 填充像素
    if (image_.elemSize() == 1) 
        func.operator()<uchar>();
    else if (image_.elemSize() == 3) 
        func.operator()<BGRPixel>();

    return *this;
}

nl::ImageOperator& nl::ImageOperator::set_image_width(int width) {
    if (image_.empty())
        return *this;
    double multiple = width * 1.0 / image_.cols;

    auto func = [&]<typename T>() {
        int new_width = image_.cols * multiple, new_height = image_.rows;
        auto new_image = cv::Mat(image_.rows, new_width, image_.type());
        auto [src_data, src_row, src_col] = GetImageData<T>(image_);
        auto [new_data, ignore1, ignore2] = GetImageData<T>(new_image);

        for (int i = 0; i < new_height; ++i)
            for (int j = 0; j < new_width; ++j)
                new_data({ i,j }) = src_data({ i,static_cast<int>(j / multiple) });

        image_ = new_image;
    };

    if (image_.elemSize() == 1)
        func.operator() < uchar > ();
    else if (image_.elemSize() == 3)
        func.operator() < BGRPixel > ();

}

nl::ImageOperator& nl::ImageOperator::set_image_height(int height) {
    if (image_.empty())
        return *this;
    double multiple = height * 1.0 / image_.rows;

    auto func = [&]<typename T>() {
        int new_height = image_.rows * multiple, new_width = image_.cols;
        auto new_image = cv::Mat(image_.rows, new_width, image_.type());
        auto [src_data, src_row, src_col] = GetImageData<T>(image_);
        auto [new_data, ignore1, ignore2] = GetImageData<T>(new_image);

        for (int i = 0; i < new_height; ++i)
            for (int j = 0; j < new_width; ++j)
                new_data({ i,j }) = src_data({ static_cast<int>(i / multiple), j });

        image_ = new_image;
    };
    if (image_.elemSize() == 1)
        func.operator() < uchar > ();
    else if (image_.elemSize() == 3)
        func.operator() < BGRPixel > ();
}


nl::ImageOperator& nl::ImageOperator::resize(int width, int height) {
    set_image_width(width);
    set_image_height(height);
    return *this;
}

nl::ImageOperator& nl::ImageOperator::rotation(int angle) {
    rotation(image_.rows / 2, image_.cols / 2, angle);
    return *this;
}

nl::ImageOperator& nl::ImageOperator::rotation(int x, int y, int angle) {
    angle = -angle;
    if (image_.elemSize() == 1) {

    }
    else if (image_.elemSize() == 3) {
        auto [src_data, row, col] = GetImageData<BGRPixel>(image_);
        double angle_rad = angle * CV_PI / 180;
        cv::Mat image(row, col, image_.type());
        auto [dest_data, _1, _2] = GetImageData<BGRPixel>(image);
        /*
			dest.x = (src.x - x) * cos(angle) - (src.y - y) * sin(angle) + x
            dest.y = (src.x - x) * sin(angle) + (src.y - y) * cos(angle) + y
        */
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                int dx = i - x;
                int dy = j - y;
                int dest_x = static_cast<int>(dx * cos(angle_rad) - dy * sin(angle_rad) + x);
                int dest_y = static_cast<int>(dx * sin(angle_rad) + dy * cos(angle_rad) + y);
                if (dest_x >= 0 && dest_x < row && dest_y >= 0 && dest_y < col) {
                    dest_data({ dest_x, dest_y }) = src_data({ i, j });
                }
            }

        }
        image_ = image;
    }

    return *this;
}

/*
 * @brief : 水平翻转
*/
nl::ImageOperator& nl::ImageOperator::reverse_horizontally() {
    auto [data, row, col] = GetImageData<BGRPixel>(image_);

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

    auto [data, row, col] = GetImageData<BGRPixel>(image_); 

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
        auto [data, row, col] = GetImageData<uchar>(image_); 
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
        auto [data, row, col] = GetImageData<BGRPixel>(image_);
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
nl::ImageOperator::operator bool() const {
    return !image_.empty();
}

void nl::ImageOperator::show_image() {
    cv::imshow("ImageShowWindow", image_);
}

