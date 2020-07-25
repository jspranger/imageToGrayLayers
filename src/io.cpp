#include "io.h"

#include "utils.h"

// Declare private prototypes
void showHelpMenu();
void failWithError(std::string errorDescription);

// Define public procedures
void IO::parseInput(int argc, char** argv, InputData& inputData)
{
	// Setup default values
	inputData.outputFolder = std::filesystem::current_path();
	inputData.equalizeHistogram = false;
	inputData.exportGradients = false;
	inputData.accumulateIntensity = false;
	inputData.clusters = 8;

	// Validate and read input parameters
	for (int i = 1; i < argc; ++i)
	{
		std::string iParam = std::string(argv[i]);

		// If help menu is requested, show menu and exit immediately
		if (Utils::compareStrings(iParam, std::string("--help")) || Utils::compareStrings(iParam, std::string("-h")))
		{
			showHelpMenu();
			exit(EXIT_SUCCESS);
		}

		// Locate required input image parameter
		if (Utils::compareStrings(iParam, std::string("-i")))
		{
			// Expect input image path to exist
			if (i + 1 >= argc)
			{
				failWithError("Error: Missing mandatory image file path!");
				exit(EXIT_FAILURE);
			}

			// Try to read image file
			std::string imageFilePath = std::string(argv[i + 1]);
			inputData.inputImage = cv::imread(imageFilePath, cv::IMREAD_COLOR);
			if (!inputData.inputImage.data)
			{
				failWithError("Error: Could not read image file!");
				exit(EXIT_FAILURE);
			}
		}

		// Locate number of layers parameter
		if (Utils::compareStrings(iParam, std::string("-l")))
		{
			// Expect number to exist
			if (i + 1 >= argc)
			{
				failWithError("Error: Missing number of layers!");
				exit(EXIT_FAILURE);
			}

			// Expect number to be parsable
			char* endPtr;
			long numberOfLayers = strtol(argv[i + 1], &endPtr, 10);
			if (endPtr == nullptr)
			{
				failWithError("Error: Number of layers has unexpected format!");
				exit(EXIT_FAILURE);
			}

			// Expect number to be within established limits
			if (numberOfLayers < 2 || numberOfLayers > 256)
			{
				failWithError("Error: Invalid number of layers!");
				exit(EXIT_FAILURE);
			}

			inputData.clusters = int(numberOfLayers);
		}

		// Locate contrast enhancement parameter
		if (Utils::compareStrings(iParam, std::string("-e")))
		{
			inputData.equalizeHistogram = true;
		}

		// Locate export gradients parameter
		if (Utils::compareStrings(iParam, std::string("-g")))
		{
			inputData.exportGradients = true;
		}

		// Locate accumulate intensity parameter
		if (Utils::compareStrings(iParam, std::string("-a")))
		{
			inputData.accumulateIntensity = true;
		}

		// Locate output folder parameter
		if (Utils::compareStrings(iParam, std::string("-o")))
		{
			// Expect path to exist
			if (i + 1 >= argc)
			{
				failWithError("Error: Missing folder path!");
				exit(EXIT_FAILURE);
			}

			// Check if output folder exists, if not, create it
			std::filesystem::path output = std::filesystem::path(argv[i + 1]);
			if (!std::filesystem::is_directory(std::filesystem::status(output)))
			{
				if (!std::filesystem::create_directory(output))
				{
					failWithError("Error: Could not create output directory!");
					exit(EXIT_FAILURE);
				}
			}

			inputData.outputFolder = output;
		}
	}

	// If missing mandatory parameter, generate error
	if (!inputData.inputImage.data)
	{
		failWithError("Error: Missing mandatory image file path!");
		exit(EXIT_FAILURE);
	}
}

void IO::writeOutput(const OutputData& outputData)
{
	if (!cv::imwrite(outputData.filePath, outputData.outputImage))
	{
		failWithError("Error: Could not write output image!");
		exit(EXIT_FAILURE);
	}
}

// Define private prototypes
void showHelpMenu()
{
	std::cout << std::endl;
	std::cout << "Example usage\n" << std::endl;
	std::cout << "imageToGrayLayers [-h] [-e] [-g] [-l 8] [-o /path/to/output/folder] -i /path/to/image/file\n\n" << std::endl;
	std::cout << "About\n" << std::endl;
	std::cout << "This application converts an image to grayscale, then it segments a set amount of regions of similar pixel intensity and stores each region into a new image." << std::endl;
	std::cout << "The main purpose for this application is to generate gray layers which can be used by laser engravers, running at different power settings for each pass, in order to create a shading effect.\n\n" << std::endl;
	std::cout << "Parameters\n" << std::endl;
	std::cout << "-i <file>\t[required]\tPath to the input image file." << std::endl;
	std::cout << "-e\t\t[optional]\tIf set, improves image contrast. Defaults to not set." << std::endl;
	std::cout << "-g\t\t[optional]\tIf set, exports the image gradient values in the layers, which can be used for dithering. Defaults to not set." << std::endl;
	std::cout << "-a\t\t[optional]\tIf set, exported layers accumulate intensity values. The order of exporting is reversed. Defaults to not set." << std::endl;
	std::cout << "-l <layers>\t[optional]\tNumber of layers to output (between 2 and 256). If not set, defaults to 8." << std::endl;
	std::cout << "-o <folder>\t[optional]\tPath to the output folder. If not set, files will be output to the current folder." << std::endl;
	std::cout << "-h (--help)\t[optional]\tDisplays this help menu and exits." << std::endl;
}

void failWithError(std::string errorDescription)
{
	std::cerr << errorDescription << "\n" << std::endl;
	showHelpMenu();
}
