/*
* Created 29th May 2018
*/

#include "enums.h"
#include "PixelMatrix.h"

#include <memory>

class PixelMatrix;

class BitmapImage {
	
	public:
		/**
		* Constructs a bitmap image from a file
		*/
		BitmapImage(std::string filename);
		
		/**
		* Reads and stores the pixel matrix from the image file
		*/
		void ReadPixelMarix();
		
		/**
		*
		*/
		std::string HeaderString();
		
		/**
		*
		*/
		std::string PixelMatrixString() const;
		
		/**
		*
		*/
		std::string PrintASCIIPixelMatrix(bool dark = false);

		/**
		 * 
		 */
		int GetGreyScalePixelValue(const int x, const int y) const;

		/**
		 * 
		 */
		double GetAverageGreyScaleValue() const;
		
		/**
		* Getter methods
		*/
		std::string GetFilename() const { return m_filename; }
		FileType GetFileType() const { return m_fileType; }
		int GetFileSize() const { return m_fileSize; }
		int GetPixelOffset() const { return m_pixelOffset; }
		int GetHeaderSize() const { return m_headerSize; }
		BitmapHeaderType GetBitmapHeaderType() const { return SizeToBitmapHeaderType(m_headerSize); }
		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		int GetColourPlanes() const { return m_colourPlanes; }
		int GetBpp() const { return m_bpp; }
		int GetCompressionMethod() const { return m_compressionMethod; }
		int GetRowSize() const { return m_rowSize; }
		int GetPixelMatrixSize() const { return m_pixelMatrixSize; }
		bool IsFlipHeight() const { return m_flipHeight; }
		
	private:
	
		/**
		* Reads and stores the metadata from the bitmap's header
		*/
		void ReadHeader();

		/**
		 * 
		 */
		void ReadBitmapInfoHeader(std::ifstream& fileStream);

		/**
		 * 
		 */
		void ReadBitmapCoreHeader(std::ifstream& fileStream);
		
		/**
		*
		*/
		void ReadPixelMarix4Bpp(FILE* filePointer);
		void ReadPixelMarix24Or32Bpp(FILE* filePointer);
		
		std::string m_filename;
		FileType m_fileType = FileType::Invalid;
		BitmapHeaderType m_headerType = BitmapHeaderType::Unknown;
		int m_fileSize = 0;
		int m_pixelOffset = 0;
		int m_headerSize = 0;
		int m_width = 0;
		int m_height = 0;
		bool m_flipHeight = false;
		int m_colourPlanes = 0;
		int m_bpp = 0;
		int m_compressionMethod = 0;
		int m_rowSize = 0;
		int m_pixelMatrixSize = 0;
		
		std::unique_ptr<PixelMatrix> m_pixelMatrix = nullptr;
};