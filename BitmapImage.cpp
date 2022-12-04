/*
* Created 29th May 2018
*/

#include "BitmapImage.h"

#include <fstream>
#include <iostream>
#include <sstream>

BitmapImage::BitmapImage(std::string filename)
	: m_filename(filename)
{
	ReadHeader();
	m_pixelMatrix = std::make_unique<PixelMatrix>(m_height, m_width);
}

void BitmapImage::ReadHeader()
{
	std::ifstream fileStream{m_filename, std::ios::binary};
	if (!fileStream.is_open())
	{
    	std::cerr << "Could not open file.\n";
		throw std::runtime_error("Could not open file");
	}

	// file type
	char* fileType = new char[3];
	fileStream.read(fileType, 2);
	fileType[2] = '\0';
	m_fileType = StringToFileType(fileType);
	if (m_fileType != FileType::Bitmap)
	{
    	std::cerr << "Only Bitmap format is supported.\n" 
			<< "Given file was of type '" << fileType << "'.\n";
		throw std::runtime_error("File is not in bitmap format");
	}
	delete[] fileType;

	// size of bitmap file in bytes
	fileStream.read(reinterpret_cast<char*>(&m_fileSize), 4);
	
	// pass over reserved slots
	int reservedSlots;
	fileStream.read(reinterpret_cast<char*>(&reservedSlots), 4);
	
	// size of bitmap file in bytes
	fileStream.read(reinterpret_cast<char*>(&m_pixelOffset), 4);
	 
	// header size
	fileStream.read(reinterpret_cast<char*>(&m_headerSize), 4);
	
	m_headerType = SizeToBitmapHeaderType(m_headerSize);
	switch (m_headerType)
	{
		case BitmapHeaderType::BitmapInfoHeader:
		case BitmapHeaderType::BitmapV4Header:
		case BitmapHeaderType::BitmapV5Header:
			ReadBitmapInfoHeader(fileStream);
			break;
		case BitmapHeaderType::BitmapCoreHeader_OS21XBitmapHeader:
			ReadBitmapCoreHeader(fileStream);
			break;
		default:
    		fileStream.close();
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

    fileStream.close();
}

void BitmapImage::ReadBitmapInfoHeader(std::ifstream& fileStream)
{
	// width of image
	fileStream.read(reinterpret_cast<char*>(&m_width), 4);
	
	// height of image
	fileStream.read(reinterpret_cast<char*>(&m_height), 4);
	if (m_height < 0) 
	{
		m_flipHeight = true;
		m_height *= -1;
	}
	
	// number of colour planes
	fileStream.read(reinterpret_cast<char*>(&m_colourPlanes), 2);
	
	// the number of bits per pixel
	fileStream.read(reinterpret_cast<char*>(&m_bpp), 2);
	if (m_bpp < 0) m_bpp = pow(2, 2*8) + m_bpp;
	
	// the compression method being used
	// NOTE: Non-zero compression methods may cause unintended behaviour.
	fileStream.read(reinterpret_cast<char*>(&m_compressionMethod), 4);
	
	// go to the end of the header
	unsigned int tmp = 0;
	for (int i = 0; i < 5; i++) {
		fileStream.read(reinterpret_cast<char*>(&tmp), 4);
	}
}

void BitmapImage::ReadBitmapCoreHeader(std::ifstream& fileStream)
{
	// width of image
	fileStream.read(reinterpret_cast<char*>(&m_width), 2);

	// height of image
	fileStream.read(reinterpret_cast<char*>(&m_height), 2);
	
	// number of colour planes
	fileStream.read(reinterpret_cast<char*>(&m_colourPlanes), 2);
	
	// The number of bits per pixel
	fileStream.read(reinterpret_cast<char*>(&m_bpp), 2);
} 

void BitmapImage::ReadPixelMarix()
{
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
	
	for (int i = m_height - 1; i > -1; i--) {
		
		char* tmp = new char[m_rowSize];
		fread(tmp, psize, m_rowSize / psize, filePointer);
 
		for (int j = 0; j < m_rowSize; j++) {
			// For each byte in the row
			unsigned int firstGreyScaleValue = ((*tmp) >> 4) % 16;
			Pixel p1{firstGreyScaleValue};
			m_pixelMatrix->SetPixelAtPosition(p1, i, j * 2);
			
			unsigned int secondGreyScaleValue = (*tmp) % 16;
			Pixel p2{secondGreyScaleValue};
			m_pixelMatrix->SetPixelAtPosition(p2, i, j * 2 + 1);
			
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
	
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			unsigned int pixel;
			fread(&pixel, elementSize, 1, filePointer);

			const unsigned int blue = (256 + pixel % 256) % 256;
			const unsigned int green = (256 + (pixel >> 8) % 256) % 256;
			const unsigned int red = (256 + (pixel >> 16) % 256) % 256;
			Pixel p{red, green, blue};

			const unsigned int xCoord = x;
			const unsigned int yCoord = (m_flipHeight) ? y : m_height - y - 1;
			m_pixelMatrix->SetPixelAtPosition(p, xCoord, yCoord);

			if (m_bpp == 24 && x == m_width - 1)
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

std::string BitmapImage::PrintASCIIPixelMatrix(bool dark)
{
	std::ostringstream oss;
	
	if (m_bpp == 24 || m_bpp == 32) 
	{
		char light[18][4] = {
			"WWW", "000", "QQQ", "AAA", "WW ", "00 ", "QQ ", "AA ", "$$ ", "oo ",
			">> ", "\"\" ", "\'\' ", ",, ", ":: ", "-- ", "__ ", ".. "
		};
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				int greyScaleValue = m_pixelMatrix->GetGreyScalePixelValue(x, y);
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

const PixelMatrix& BitmapImage::GetPixelMatrix() const
{
	return *m_pixelMatrix;
}