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
	Unsupported,
	BitmapInfoHeader,
	BitmapCoreHeader_OS21XBitmapHeader,
	BitmapV4Header
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