/**
* Created Sat 13th October 2018
*/

#include "MosaicImage.h"

#include "BitmapImage.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

MosaicImage::MosaicImage(std::string pixelImageDir)
: m_pixelImageDir(pixelImageDir)
{
    ReadPixelListFile();
}

std::string MosaicImage::PixelImageListToString()
{
	std::ostringstream oss;
    oss << "\n";
    int i = 0;
	
	for (auto list : m_pixelImageList)
	{
        oss << "[" << i++ << "]\t";
        for (auto pixel : list.second)
        {
		    oss << pixel << "\t";
        }
        oss << "\n";
	}
	
	return oss.str();
}

void MosaicImage::ReadPixelListFile()
{
	std::ifstream pixelListFile;
	std::string line;
	pixelListFile.open(m_pixelImageDir + "pixelList.txt");

	if (pixelListFile.is_open())
	{
		while (getline(pixelListFile,line))
		{
			std::istringstream lineStream(line);
			std::string key;
			getline(lineStream, key, '\t');
			m_pixelImageList[key] = std::vector<std::string> {};

			std::string value;
			while (getline(lineStream, value, '\t'))
			{
				m_pixelImageList[key].push_back(value);
			}
		}
		pixelListFile.close();
	}
	else 
	{
		std::cerr << "Unable to open file " << m_pixelImageDir + "pixelList.txt" << "\n"; 
	}

}

std::string MosaicImage::FindClosestValue(const std::string& value)
{
	if (m_pixelImageList.find(value) != m_pixelImageList.end())
	{
		return value;
	}

	int intValue = std::stoi(value);
	int minIntValue = intValue;
	int maxIntValue = intValue;
	bool closestFound = false;
	while (!closestFound)
	{
		minIntValue--;
		maxIntValue++;
		if (m_pixelImageList.find(std::to_string(minIntValue)) != m_pixelImageList.end())
		{
			return std::to_string(minIntValue);
		}
		if (m_pixelImageList.find(std::to_string(maxIntValue)) != m_pixelImageList.end())
		{
			return std::to_string(maxIntValue);
		}
	}
	return "";
}

void MosaicImage::CreateMosaicImage(BitmapImage image)
{
	int pixelSize = 100;

	const int imageHeight = image.GetHeight();
	const int imageWidth = image.GetWidth();
	
	const int superHeight = pixelSize * imageHeight;
	const int superWidth = pixelSize * imageWidth;

	std::vector<int> superPixelMatrix(superHeight*superWidth, 0);
	
	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			std::string greyscaleValue = std::to_string(image.GetGreyScalePixelValue(i, j));
			greyscaleValue = FindClosestValue(greyscaleValue);

			srand(time(NULL));
			
			int index = rand() % m_pixelImageList[greyscaleValue].size();
			const std::string pixelImageName = m_pixelImageList[greyscaleValue][index];
			
			std::ostringstream filename;
			filename << m_pixelImageDir << pixelImageName;

			BitmapImage pixelImage = BitmapImage(filename.str());
			pixelImage.ReadPixelMarix();
			
			for (int ii = 0; ii < pixelImage.GetHeight(); ii++) {
				for (int jj = 0; jj < pixelImage.GetWidth(); jj++) {
					int greyscaleValue = pixelImage.GetGreyScalePixelValue(ii, jj);
					int index = (ii + 100 * i) + superHeight * (jj + 100 * j);
					superPixelMatrix.at(index) = greyscaleValue;
				}
			}
		}
	}
	
	std::ostringstream newFilename;
	std::string imageFilename = image.GetFilename();
	newFilename << "Mosaic_" << imageFilename.substr(imageFilename.find_last_of("/")+1);
	CreateImageFile(newFilename.str(), superHeight, superWidth, superPixelMatrix);
}

void MosaicImage::CreateImageFile(std::string imageFileName, int height, int width, std::vector<int> pixelMatrix)
{
	unsigned short bpp = 24;
	
	/*************************************************************************************
	 *									Header
	 ************************************************************************************/
// 	"BM(4-byte file size)(2-bytes)(2-bytes)(4-byte offset)"
// 	"(4-byte size of this header 40)(2-byte width in pixels)(2-byte height in pixels)(2-byte 1)(2-byte bpp)"
	FILE *fp = fopen(imageFileName.c_str(), "w");
	const char* str = "BM";
	fwrite(str, sizeof(char), 2, fp);
	
	unsigned int rowSize = std::floor( ( bpp * width + 31 ) / 32 ) * 4;

	unsigned int filesize = rowSize * height + 54;
	fwrite(&filesize, 4, 1, fp);
	
	unsigned int reserved = 0;
	fwrite(&reserved, 4, 1, fp);
	
	unsigned int offset = 54;
	fwrite(&offset, 4, 1, fp);
	
	// DIB Header
	unsigned int headerSize = 40;
	fwrite(&headerSize, 4, 1, fp);
	
	unsigned int unsignedWidth = width;
	fwrite(&unsignedWidth, 4, 1, fp);
	
	int unsignedHeight = -1 * height;
	fwrite(&unsignedHeight, 4, 1, fp);
	
	unsigned short colourPlanes = 1;
	fwrite(&colourPlanes, 2, 1, fp);
	
	fwrite(&bpp, 2, 1, fp);
	
	unsigned int compression = 0;
	fwrite(&compression, 4, 1, fp);
		
	unsigned int imageSize = 0;
	fwrite(&imageSize, 4, 1, fp);
		
	int horizontalResolution = 2835;
	fwrite(&horizontalResolution, 4, 1, fp);	
	
	int verticalResolution = 2835;
	fwrite(&verticalResolution, 4, 1, fp);	
	
	unsigned int numColours = 0;
	fwrite(&numColours, 4, 1, fp);	
	
	unsigned int importantColours = 0;
	fwrite(&importantColours, 4, 1, fp);
	
	/*************************************************************************************
	 *									Body
	 ************************************************************************************/
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < rowSize/3; j++) {
			if (j < width) {
				char greyscaleValue = pixelMatrix[i + j*width];
				char pixel[] = {greyscaleValue, greyscaleValue, greyscaleValue};
				fwrite(pixel, 1, 3, fp);
			} else {
				char pixel[] = {'\0', '\0', '\0'};
				fwrite(pixel, 1, 3, fp);
			}
		}
	}
	
	fclose(fp);
}