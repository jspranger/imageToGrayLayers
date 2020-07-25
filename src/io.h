#pragma once

#include <opencv2/opencv.hpp>
#include <filesystem>

namespace IO
{
	struct InputData
	{
		cv::Mat inputImage;
		std::filesystem::path outputFolder;
		bool equalizeHistogram;
		bool exportGradients;
		bool accumulateIntensity;
		int clusters;
	};

	struct OutputData
	{
		cv::Mat outputImage;
		std::string filePath;
	};

	void parseInput(int argc, char** argv, InputData& inputData);
	void writeOutput(const OutputData& outputData);
}
