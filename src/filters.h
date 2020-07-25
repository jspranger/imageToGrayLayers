#pragma once

#include <opencv2/opencv.hpp>

namespace Filters
{
	cv::Mat histogramEqualization(const cv::Mat& input);
	cv::Mat kMeansSegmentation(const cv::Mat& input, int k);
}
