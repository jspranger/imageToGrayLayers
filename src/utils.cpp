#include "utils.h"

bool Utils::compareStrings(const std::string& str1, const std::string& str2, bool caseSensitive)
{
	return ((str1.size() == str2.size()) && std::equal(str1.begin(),
		str1.end(),
		str2.begin(),
		[caseSensitive](const char& c1, const char& c2) {
		bool equals = c1 == c2;

		if (!caseSensitive)
			equals |= std::tolower(c1) == std::tolower(c2);

		return equals;
	}));
}

std::vector<unsigned char> Utils::unique(const cv::Mat& input, bool sort)
{
	std::vector<unsigned char> output;

	if (input.channels() > 1 || input.type() != CV_8UC1)
	{
		return output;
	}

	for (int y = 0; y < input.rows; ++y)
	{
		const unsigned char* row_ptr = input.ptr<unsigned char>(y);

		for (int x = 0; x < input.cols; ++x)
		{
			unsigned char value = row_ptr[x];

			if (std::find(output.begin(), output.end(), value) == output.end())
			{
				output.push_back(value);
			}
		}
	}

	if (sort)
	{
		std::sort(output.begin(), output.end());
	}

	return output;
}
