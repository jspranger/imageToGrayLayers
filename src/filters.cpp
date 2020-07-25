#include "filters.h"

cv::Mat Filters::histogramEqualization(const cv::Mat& input)
{
	// Convert BGR image to YCrCb colorspace and split channels
	cv::Mat yCrCbInputImage, yCrCbChannels[3];

	cv::cvtColor(input, yCrCbInputImage, cv::COLOR_BGR2YCrCb);
	cv::split(yCrCbInputImage, yCrCbChannels);

	// Equalize value channel
	cv::Mat yCrCbEqualizedValueChannel;
	cv::equalizeHist(yCrCbChannels[0], yCrCbEqualizedValueChannel);

	// Merge channels and convert back from YCrCb to BGR colorspace
	std::vector<cv::Mat> yCrCbEqualizedChannels = { yCrCbEqualizedValueChannel, yCrCbChannels[1], yCrCbChannels[2] };
	cv::Mat yCrCbEqualizedImage, equalizedImage;

	cv::merge(yCrCbEqualizedChannels, yCrCbEqualizedImage);
	cv::cvtColor(yCrCbEqualizedImage, equalizedImage, cv::COLOR_YCrCb2BGR);

	return equalizedImage;
}

cv::Mat Filters::kMeansSegmentation(const cv::Mat& input, int k)
{
	int size = input.rows * input.cols;
	cv::Mat data = input.reshape(1, size);
	data.convertTo(data, CV_32F);

	std::vector<int> labels;
	cv::Mat1f colors;

	cv::kmeans(data,
		k,
		labels,
		cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.),
		3,
		cv::KMEANS_PP_CENTERS,
		colors
	);

	for (unsigned int i = 0; i < size; i++)
	{
		data.at<float>(i) = colors(labels[i]);
	}

	cv::Mat outputImage = data.reshape(1, input.rows);
	outputImage.convertTo(outputImage, CV_8UC1);

	return outputImage;
}
