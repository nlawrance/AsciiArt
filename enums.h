/**
* Created 5th June 2018
*/

#include <string>
#include <vector>

enum FileType {
	INVALID,
	BITMAP
};

enum BitmapHeaderType {
	UNSUPPORTED,
	BITMAPINFOHEADER,
	BITMAPCOREHEADER_OS21XBITMAPHEADER,
	BITMAPV4HEADER
};

FileType StringToFileType(std::string string);

std::string FileTypeToString(FileType fileType);

BitmapHeaderType SizeToBitmapHeaderType(unsigned int size);

std::string BitmapHeaderTypeToString(BitmapHeaderType bitmapHeaderType);