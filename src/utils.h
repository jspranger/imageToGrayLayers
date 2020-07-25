#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

namespace Utils
{
	bool compareStrings(const std::string& str1, const std::string& str2, bool caseSensitive = false);
	std::vector<unsigned char> unique(const cv::Mat& input, bool sort = false);
}
