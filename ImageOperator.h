#pragma once

#include <array>
#include <filesystem>

#include <opencv.hpp>
#include "MultArray.hpp"

namespace nl {

class ImageOperator {
	struct RGBPixel {
		uchar R, G, B;
	};

	HWND window_handle_;
	cv::Mat image_;

	template<typename T>
	constexpr size_t get_elem_size();

public:
	
	ImageOperator();
	explicit ImageOperator(const std::filesystem::path& path);
	~ImageOperator();

	// @TODO 拷贝动作待处理
	ImageOperator(const ImageOperator&) = delete;
	ImageOperator operator = (const ImageOperator&) = delete;

	template<typename T>
	nl::MultArray<T> get_image_data();

	HWND get_show_window();

	void open(const std::filesystem::path& path);
	void save();

	ImageOperator& zoom(int multiple);
	ImageOperator& resize(int width, int height);
	ImageOperator& rotation(int angle);
	ImageOperator& rotation(int angle, int x, int y);
	ImageOperator& reverse_horizontally();
	ImageOperator& reverse_vertically();
	ImageOperator& to_grayscale();
	ImageOperator& to_binary(int = 100);
	ImageOperator& to_pseudo_color();
	std::array<size_t, 256> get_histogram_data();
	void show_image();


};

} // namespace nl

