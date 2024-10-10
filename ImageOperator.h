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
	// ImageOperator(uchar image[], size_t row, size_t column);
	~ImageOperator();

	void open(const std::filesystem::path& path);
	void save();

	ImageOperator& zoom(int multiple);
	ImageOperator& resize(int width, int height);
	ImageOperator& rotation(int angle);
	ImageOperator& rotation(int angle, int x, int y);
	ImageOperator& to_grayscale();
	ImageOperator& to_binary();
	ImageOperator& to_pseudo_color();
	std::array<size_t, 256> get_histogram();
	HWND get_show_window();
	void show_image();


};

} // namespace nl

