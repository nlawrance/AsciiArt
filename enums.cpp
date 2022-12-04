/**
* Created 5th June 2018
*/

#include "enums.h"

FileType StringToFileType(std::string string)
{
	std::vector<std::string> bitmapFormats = {"BM", "BA", "CI", "CP", "IC", "PT"};
	for (std::string format : bitmapFormats)
	{
		if (string == format)
		{
			return BITMAP;
		}
	}
	return INVALID;
}

std::string FileTypeToString(FileType fileType)
{
	if (fileType == BITMAP)
	{
		return "BITMAP";
	}
	else if (fileType == INVALID)
	{
		return "INVALID";
	}
	return "none";
}

BitmapHeaderType SizeToBitmapHeaderType(unsigned int size)
{
	if (size == 40 || size == 124)
	{
		return BitmapHeaderType::BITMAPINFOHEADER;
	}
	else if (size == 12)
	{
		return BitmapHeaderType::BITMAPCOREHEADER_OS21XBITMAPHEADER;
	}
	else if (size == 108)
	{
		return BitmapHeaderType::BITMAPV4HEADER;
	}
	return BitmapHeaderType::UNSUPPORTED;
}

std::string BitmapHeaderTypeToString(BitmapHeaderType bitmapHeaderType)
{
	switch (bitmapHeaderType)
	{
		case BitmapHeaderType::BITMAPINFOHEADER:
			return "BITMAPINFOHEADER";
		case BitmapHeaderType::BITMAPCOREHEADER_OS21XBITMAPHEADER:
			return "BITMAPCOREHEADER or OS21XBITMAPHEADER";
		case BitmapHeaderType::BITMAPV4HEADER:
			return "BITMAPV4HEADER";
		default:
			return "UNSUPPORTED";
	}
}