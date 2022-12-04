/**
* Created 5th June 2018
*/

#include "enums.h"

FileType StringToFileType(std::string string)
{
	const static std::vector<std::string> BitmapFormats = { "BM", "BA", "CI", "CP", "IC", "PT" };
	for (const std::string& format : BitmapFormats)
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
		case 12:
			return BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader;
		case 16:
			return BitmapHeaderType::OS22XBitmapHeader;
		case 40:
			return BitmapHeaderType::BitmapInfoHeader;
		case 52:
			return BitmapHeaderType::BitmapV2InfoHeader;
		case 56:
			return BitmapHeaderType::BitmapV3InfoHeader;
		case 64:
			return BitmapHeaderType::OS22XBitmapHeader;
		case 108:
			return BitmapHeaderType::BitmapV4Header;
		case 124:
			return BitmapHeaderType::BitmapV5Header;
		default:
			return BitmapHeaderType::Unknown;
	}
}

std::string BitmapHeaderTypeToString(BitmapHeaderType bitmapHeaderType)
{
	switch (bitmapHeaderType)
	{
		case BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader:
			return "BITMAPCOREHEADER or OS21XBITMAPHEADER";
		case BitmapHeaderType::BitmapInfoHeader:
			return "BITMAPINFOHEADER";
		case BitmapHeaderType::BitmapV2InfoHeader:
			return "BITMAPV2INFOHEADER";
		case BitmapHeaderType::BitmapV3InfoHeader:
			return "BITMAPV3INFOHEADER";
		case BitmapHeaderType::BitmapV4Header:
			return "BITMAPV4HEADER";
		case BitmapHeaderType::BitmapV5Header:
			return "BITMAPV5HEADER";
		case BitmapHeaderType::OS22XBitmapHeader:
			return "OS22XBITMAPHEADER";
		case BitmapHeaderType::Unknown:
		default:
			return "Unknown";
	}
}