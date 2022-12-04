/**
* Created 5th June 2018
*/

#include <string>
#include <vector>

/**
 * 
 */
enum class FileType {
	Invalid,
	Bitmap
};

/**
 * 
 */
enum class BitmapHeaderType {
	BitmapCoreHeader_OS21XBitmapHeader,
	BitmapInfoHeader,
	BitmapV2InfoHeader,
	BitmapV3InfoHeader,
	BitmapV4Header,
	BitmapV5Header,
	OS22XBitmapHeader,
	Unknown,
};

/**
 * 
 */
FileType StringToFileType(std::string string);

/**
 * 
 */
std::string FileTypeToString(FileType fileType);

/**
 * 
 */
BitmapHeaderType SizeToBitmapHeaderType(unsigned int size);

/**
 * 
 */
std::string BitmapHeaderTypeToString(BitmapHeaderType bitmapHeaderType);