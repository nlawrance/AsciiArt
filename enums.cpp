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
			return FileType::Bitmap;
		}
	}
	return FileType::Invalid;
}

std::string FileTypeToString(FileType fileType)
{
	switch (fileType)
	{
		case FileType::Bitmap:
			return "BITMAP";
		case FileType::Invalid:
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
			return BitmapHeaderType::BitmapInfoHeader;
		case 12:
			return BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader;
		case 108:
			return BitmapHeaderType::BitmapV4Header;
		default:
			return BitmapHeaderType::Unsupported;
	}
}

std::string BitmapHeaderTypeToString(BitmapHeaderType bitmapHeaderType)
{
	switch (bitmapHeaderType)
	{
		case BitmapHeaderType::BitmapInfoHeader:
			return "BITMAPINFOHEADER";
		case BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader:
			return "BITMAPCOREHEADER or OS21XBITMAPHEADER";
		case BitmapHeaderType::BitmapV4Header:
			return "BITMAPV4HEADER";
		default:
			return "UNSUPPORTED";
	}
}