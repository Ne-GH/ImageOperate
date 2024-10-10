#pragma once

#include <array>
#include <filesystem>

#include <opencv.hpp>

namespace nl {

class ImageOperator {

	cv::Mat image_;
	HWND window_handle_;

public:

	ImageOperator();
	explicit ImageOperator(const std::filesystem::path& path);
	~ImageOperator();

	// @TODO 拷贝动作待处理
	ImageOperator(const ImageOperator&) = delete;
	ImageOperator operator = (const ImageOperator&) = delete;

	void open(const std::filesystem::path& path);
	void save();

	ImageOperator& zoom(int multiple);
	ImageOperator& resize(int width, int height);
	ImageOperator& rotation(int angle);
	ImageOperator& rotation(int angle, int x, int y);
	ImageOperator& reverse_horizontally();
	ImageOperator& reverse_vertically();
	ImageOperator& to_grayscale();
	ImageOperator& to_binary();
	ImageOperator& to_pseudo_color();
	std::array<size_t, 256> get_histogram_data();
	HWND get_show_window();
	void show_image();


};

} // namespace nl

