/*
* Created 29th May 2018
*/

#include "BitmapImage.h"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

BitmapImage::BitmapImage(std::string filename)
	: m_filename(filename)
{
	ReadHeader();
}

void BitmapImage::ReadHeader()
{
	FILE* filePointer = fopen(m_filename.c_str(), "r");
	if (filePointer == nullptr)
	{
    	std::cerr << "Could not open file.\n";
		throw std::runtime_error("Could not open file");
	}
	 
	// file type
	char* fileType = new char[3];
	fread(fileType, 1, 2, filePointer);
	fileType[2] = '\0';
	m_fileType = StringToFileType(fileType);
	if ( m_fileType != FileType::Bitmap )
	{
    	std::cerr << "Only Bitmap format is supported.\n" 
			<< "Given file was of type '" << fileType << "'.\n";
		throw std::runtime_error("File is not in bitmap format");
	}
	delete[] fileType;
	
	// size of bitmap file in bytes
	fread(&m_fileSize, 4, 1, filePointer);
	
	// pass over reserved slots
	int reservedSlots;
	fread(&reservedSlots, 4, 1, filePointer);
	
	// size of bitmap file in bytes
	fread(&m_pixelOffset, 4, 1, filePointer);
	 
	// header size
	fread(&m_headerSize, 4, 1, filePointer);
	
	m_headerType = SizeToBitmapHeaderType(m_headerSize);
	switch (m_headerType)
	{
		case BitmapHeaderType::BitmapInfoHeader:
		case BitmapHeaderType::BitmapV4Header:
		case BitmapHeaderType::BitmapV5Header:
			ReadBitmapInfoHeader(filePointer);
			break;
		case BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader:
			ReadBitmapCoreHeader(filePointer);
			break;
		default:
			fclose(filePointer);
			std::ostringstream oss;
			oss << "Header type " << BitmapHeaderTypeToString(m_headerType) << " ("
				<< m_headerSize << " bytes) is not supported.";
			std::cerr << oss.str() << '\n';
			throw std::runtime_error(oss.str());
	}
	
	/*
	 * size of pixel matrix
	 */
	m_rowSize = std::floor( ( m_bpp * m_width + 31 ) / 32 ) * 4;
	m_pixelMatrixSize = m_rowSize * m_height;
	
	fclose(filePointer);
}

void BitmapImage::ReadBitmapInfoHeader(FILE* filePointer)
{
	// width of image
	fread(&m_width, 4, 1, filePointer);
	
	// height of image
	fread(&m_height, 4, 1, filePointer);
	if (m_height < 0) 
	{
		m_flipHeight = true;
		m_height *= -1;
	}
	
	// number of colour planes
	fread(&m_colourPlanes, 2, 1, filePointer);
	
	// the number of bits per pixel
	fread(&m_bpp, 2, 1, filePointer);
	if (m_bpp < 0) m_bpp = pow(2, 2*8) + m_bpp;
	
	// the compression method being used
	// NOTE: Non-zero compression methods may cause unintended behaviour.
	fread(&m_compressionMethod, 4, 1, filePointer);
	
	// go to the end of the header
	unsigned int tmp = 0;
	for (int i = 0; i < 5; i++) {
		fread(&tmp, 4, 1, filePointer);
	}
}

void BitmapImage::ReadBitmapCoreHeader(FILE* filePointer)
{
	// width of image
	fread(&m_width, 2, 1, filePointer);
	
	// height of image
	fread(&m_height, 2, 1, filePointer);
	
	// number of colour planes
	fread(&m_colourPlanes, 2, 1, filePointer);
	
	// The number of bits per pixel
	fread(&m_bpp, 2, 1, filePointer);
} 

void BitmapImage::ReadPixelMarix()
{
	m_pixelMatrix = RgbMartix();
	
	FILE* filePointer = fopen(m_filename.c_str(), "r");
	if (filePointer == nullptr)
	{
    	std::cerr << "Could not open file.\n";
		throw std::runtime_error("Could not open file");
	}
	
	fseek(filePointer, m_pixelOffset, SEEK_SET);
	
	switch (m_bpp)
	{
		case 4: 
			ReadPixelMarix4Bpp(filePointer);
			break;
		case 24:
		case 32:
			ReadPixelMarix24Or32Bpp(filePointer);
			break;
		default:
			fclose(filePointer);
			std::ostringstream oss;
			oss << "BPP " << m_bpp << " is not supported.";
			std::cerr << oss.str() << '\n';
			throw std::runtime_error(oss.str());
	}
	
	fclose(filePointer);
}


void BitmapImage::ReadPixelMarix4Bpp(FILE* filePointer)
{
	// How big the chunks taken from the file will be
	int psize = 1;
	
	for (int i = m_height-1; i > -1; i--) {
		
		char* tmp = new char[m_rowSize];
		fread(tmp, psize, m_rowSize/psize, filePointer);
 
		for (int j = 0; j < m_rowSize; j++) {
			// For each byte in the row
			int firstGreyScaleValue = ((*tmp)>>4)%16;
			m_pixelMatrix.push_back(firstGreyScaleValue);
			m_pixelMatrix.push_back(firstGreyScaleValue);
			m_pixelMatrix.push_back(firstGreyScaleValue);
			
			int secondGreyScaleValue = (*tmp)%16;
			m_pixelMatrix.push_back(secondGreyScaleValue);
			m_pixelMatrix.push_back(secondGreyScaleValue);
			m_pixelMatrix.push_back(secondGreyScaleValue);
			
			// Move to the next chunk read from the file.
			tmp++;
		}
		
		delete[] tmp;
	}
}

void BitmapImage::ReadPixelMarix24Or32Bpp(FILE* filePointer)
{
	size_t elementSize = 0;
	switch (m_bpp)
	{
		case 24:
			elementSize = 3;
			break;
		case 32:
			elementSize = 4;
			break;
		default:
			fclose(filePointer);
			std::ostringstream oss;
			oss << "BitmapImage::ReadPixelMarix24Or32Bpp does not support " << m_bpp << " bpp.";
			std::cerr << oss.str() << '\n';
			throw std::runtime_error(oss.str());
	}

	m_pixelMatrix = std::vector<int>(3 * m_height * m_width, 0);
	
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			unsigned int pixel;
			fread(&pixel, elementSize, 1, filePointer);

			unsigned int blue = (256 + pixel % 256) % 256;
			unsigned int green = (256 + (pixel >> 8) % 256) % 256;
			unsigned int red = (256 + (pixel >> 16) % 256) % 256;

			int index;
			if (m_flipHeight)
			{
				index = 3 * (i * m_width + j);
			}
			else
			{
				index = 3 * ((m_height - i - 1) * m_width  + j);
			}
			
			m_pixelMatrix.at(index) = static_cast<int>(red);
			m_pixelMatrix.at(index + 1) = static_cast<int>(green);
			m_pixelMatrix.at(index + 2) = static_cast<int>(blue);

			if (m_bpp == 24 && j == m_width - 1)
			{
				// Read to the end of the line
				fseek(filePointer, m_rowSize - m_width * 3, SEEK_CUR);
			}
		}
	}
}


std::string BitmapImage::HeaderString()
{
	std::ostringstream oss;
	
	oss << "File Type: " << FileTypeToString(m_fileType) << "\n";
	oss << "File Size: " << m_fileSize << " bytes\n";
	oss << "Pixel Offset: " << m_pixelOffset << " bytes\n";
	oss << "Header Type: " << BitmapHeaderTypeToString(m_headerType) << " (" << m_headerSize << " bytes)\n";

	oss << "Flip Height: " << ((m_flipHeight) ? "true" : "false") << "\n";

	oss << "Width: " << m_width << "\n";
	oss << "Height: " << m_height << "\n";
	oss << "Colour Planes: " << m_colourPlanes << "\n";
	oss << "Bits Per Pixel: " << m_bpp << "\n";
	oss << "Comp Method: " << m_compressionMethod << "\n";
	
	oss << "Row Size: " << m_rowSize << " bytes\n";
	oss << "Pixel Matrix Size: " << m_pixelMatrixSize << " bytes\n";
	
	return oss.str();
}


std::string BitmapImage::PixelMatrixString()
{
	std::ostringstream oss;
	
	for (auto colour : m_pixelMatrix)
	{
		oss << colour << " ";
	}
	
	return oss.str();
}

int BitmapImage::RgbToGreyScale(int red, int green, int blue)
{
	return RED_WEIGHT * red + GREEN_WEIGHT * green + BLUE_WEIGHT * blue;	
}

std::string BitmapImage::PrintASCIIPixelMatrix(bool dark)
{
	std::ostringstream oss;
	
	if (m_bpp == 24 || m_bpp == 32) 
	{
		char light[18][4] = {
			"WWW", "000", "QQQ", "AAA", "WW ", "00 ", "QQ ", "AA ", "$$ ", "oo ",
			">> ", "\"\" ", "\'\' ", ",, ", ":: ", "-- ", "__ ", ".. "
		};
		for (int i = 0; i < m_height; i++)
		{
			for (int j = 0; j < m_width; j++)
			{
				int greyScaleValue = GetGreyScalePixelValue(i, j);
				double max = 255;
				
				int value = static_cast<int>(floor(greyScaleValue/max * 17));
				
				if (dark) oss << light[17 - value];
				else oss << light[value];
			}
			oss << "\n";
		}
	} 
// 	else {
// 		for (int i = 0; i < m_height; i++){
// 			for (int j = 0; j < m_width; j++) {
// 				switch (m_pixelMatrix[i][j]) {
// 					case 0:
// 						oss << "8  ";
// 						break;
// 					case 1:
// 						oss << "*  ";
// 						break;
// 					case 2:
// 						oss << ".  ";
// 						break;
// 					case 3:
// 						oss << "*  ";
// 						break;
// 					default:
// 						oss << m_pixelMatrix[i][j] << "  ";
// 						break;
// 				}
// 			}
// 			oss << std::endl;
// 		}
// 	}
	
	return oss.str();
} 

int BitmapImage::GetGreyScalePixelValue(const int x, const int y)
{
	int index = 3 * (x * m_width + y);

	int red = m_pixelMatrix[index];
	int green = m_pixelMatrix[index + 1];
	int blue = m_pixelMatrix[index + 2];

	return RgbToGreyScale(red, green, blue);
}

double BitmapImage::GetAverageGreyScaleValue()
{
	long total = 0;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++)
		{
			total += GetGreyScalePixelValue(i, j);
		}
	}
	return (total/static_cast<double>(m_width))/m_height;
}