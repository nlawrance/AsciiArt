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
			return FileType::BITMAP;
		}
	}
	return FileType::INVALID;
}

std::string FileTypeToString(FileType fileType)
{
	switch (fileType)
	{
		case FileType::BITMAP:
			return "BITMAP";
		case FileType::INVALID:
			return "INVALID";
		default:
			return "none";
	}
}

BitmapHeaderType SizeToBitmapHeaderType(unsigned int size)
{
	switch (size)
	{
		case 40:
		case 124:
			return BitmapHeaderType::BITMAPINFOHEADER;
		case 12:
			return BitmapHeaderType::BITMAPCOREHEADER_OS21XBITMAPHEADER;
		case 108:
			return BitmapHeaderType::BITMAPV4HEADER;
		default:
			return BitmapHeaderType::UNSUPPORTED;
	}
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