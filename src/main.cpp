#include "io.h"
#include "filters.h"
#include "utils.h"

// Program
int main(int argc, char** argv)
{
	// Parse input data
	IO::InputData inputData;
	IO::parseInput(argc, argv, inputData);

	// Pre-process image
	cv::Mat preProcessedImage = inputData.inputImage;

	// - Run image equalization if requested
	if (inputData.equalizeHistogram)
	{
		preProcessedImage = Filters::histogramEqualization(preProcessedImage);
	}

	// - Convert to grayscale
	cv::cvtColor(preProcessedImage, preProcessedImage, cv::COLOR_BGR2GRAY);

	// Extract regions from pre-processed image
	// - Run K-means segmentation algorithm
	cv::Mat segmentedImage = Filters::kMeansSegmentation(preProcessedImage, inputData.clusters);

	// - Find all unique regions sorted by pixel intensity (lowest to highest intensity)
	std::vector<unsigned char> uniqueRegions = Utils::unique(segmentedImage, true);

	// - Reverse order of regions vector (highest to lowest intensity)
	if (!inputData.accumulateIntensity)
	{
		std::reverse(uniqueRegions.begin(), uniqueRegions.end());
	}

	// Save each extracted region to the output folder
	// - Save the complete segmented image as preview
	IO::OutputData previewOutputData;
	previewOutputData.filePath = inputData.outputFolder.string() + "/preview.jpg";
	previewOutputData.outputImage = segmentedImage;

	if (inputData.exportGradients)
	{
		previewOutputData.outputImage = preProcessedImage;
	}
	
	IO::writeOutput(previewOutputData);

	// - Iterate each unique region
	cv::Mat accumulate = cv::Mat::ones(inputData.inputImage.rows, inputData.inputImage.cols, CV_8UC1) * 255;

	for (int i = 0; i < uniqueRegions.size(); i++)
	{
		// - Extract region and store it
		cv::Mat regionImage;
		cv::inRange(segmentedImage, uniqueRegions.at(i), uniqueRegions.at(i), regionImage);

		// - If original values of the image are requested per layer
		if (inputData.exportGradients)
		{
			cv::bitwise_and(preProcessedImage, regionImage, regionImage);
			cv::normalize(regionImage, regionImage, 0.0, 255.0, cv::NormTypes::NORM_MINMAX);
		}

		cv::bitwise_not(regionImage, regionImage);

		IO::OutputData regionOutputData;
		regionOutputData.filePath = inputData.outputFolder.string() + "/" + std::to_string(i + 1) + ".jpg";
		// - If accumulated layers are requested
		if (inputData.accumulateIntensity)
		{
			cv::bitwise_and(accumulate, regionImage, accumulate);
			regionOutputData.outputImage = accumulate;
		}
		else
		{
			regionOutputData.outputImage = regionImage;
		}

		IO::writeOutput(regionOutputData);
	}

	return EXIT_SUCCESS;
}
