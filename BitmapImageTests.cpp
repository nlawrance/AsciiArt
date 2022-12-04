/**
* Created Sat 9th June 2018
*/

#include <cppunit/extensions/HelperMacros.h>

#include "BitmapImage.h"

class BitmapImageTests : public CppUnit::TestFixture {
	 
	CPPUNIT_TEST_SUITE( BitmapImageTests );
	CPPUNIT_TEST( TestCreateBitmapImageWithHeaderSize40 );
	CPPUNIT_TEST( TestCreateBitmapImageWithHeaderSize108 );
	CPPUNIT_TEST( TestCreateBitmapImageInvalidFilename );
	CPPUNIT_TEST( TestCreateBitmapImageWithJpg );
	CPPUNIT_TEST( TestReadPixelMatrix32bpp );
	CPPUNIT_TEST( TestReadPixelMatrix32bppAsciiString );
	CPPUNIT_TEST( TestReadPixelMatrix24bpp );
	CPPUNIT_TEST_SUITE_END();

	public:
	
		~BitmapImageTests() {}
		void tearDown() {}
		void setUp() {}
		
		void TestCreateBitmapImageWithHeaderSize40() {
			std::string filename = "test_images/test4_BITMAPINFOHEADER.bmp";
			BitmapImage image = BitmapImage(filename);
			AssertHeaderMetaDataCorrect(image, filename, BITMAP, 118, 54, 40, 4, 4, 1, 32, 0, 16, 64);
		}
		
		void TestCreateBitmapImageWithHeaderSize108() {
			std::string filename = "test_images/test4_BITMAPV4HEADER.bmp";
			CPPUNIT_ASSERT_THROW_MESSAGE("BITMAPV4HEADER (108) header size is not supported.", BitmapImage image = BitmapImage(filename), std::runtime_error);
		}
		
		void TestCreateBitmapImageInvalidFilename() {
			std::string filename = "test_images/this_file_does_not_exist.bmp";
			CPPUNIT_ASSERT_THROW_MESSAGE("An exception should be thrown when the file does not exist.", BitmapImage image = BitmapImage(filename), std::runtime_error);
		}
		
		void TestCreateBitmapImageWithJpg() {
			std::string filename = "test_images/mona_lisa.jpg";
			CPPUNIT_ASSERT_THROW_MESSAGE("An exception should be thrown when the file is not a bitmap.", BitmapImage image = BitmapImage(filename), std::runtime_error);
		}
		
		void TestReadPixelMatrix32bpp() {
			std::string filename = "test_images/test4_BITMAPINFOHEADER.bmp";
			BitmapImage image = BitmapImage(filename);
			image.ReadPixelMarix();
			std::string str = "255 0 0 255 0 0 100 200 255 100 200 255 255 0 0 255 0 0 100 200 255 100 200 255 0 255 0 0 255 0 0 0 255 0 0 255 0 255 0 0 255 0 0 0 255 0 0 255 ";
			CPPUNIT_ASSERT_EQUAL_MESSAGE("Pixel matrix incorrect", str, image.PixelMatrixString());
		}
		
		void TestReadPixelMatrix32bppAsciiString() {
			std::string filename = "test_images/test4_BITMAPINFOHEADER.bmp";
			BitmapImage image = BitmapImage(filename);
			image.ReadPixelMarix();
			std::string str = "AAAAAA'' '' \nAAAAAA'' '' \n'' '' 000000\n'' '' 000000\n";
			CPPUNIT_ASSERT_EQUAL_MESSAGE("ASCII character image incorrect", str, image.PrintASCIIPixelMatrix(false));
		}
		
		void TestReadPixelMatrix24bpp() {
			std::string filename = "test_images/test10.bmp";
			BitmapImage image = BitmapImage(filename);
			image.ReadPixelMarix();
			std::string str = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 76 76 76 76 76 76 76 76 76 76 76 76 76 76 76 102 102 102 102 102 102 102 102 102 102 102 102 102 102 102 76 76 76 76 76 76 76 76 76 76 76 76 76 76 76 102 102 102 102 102 102 102 102 102 102 102 102 102 102 102 76 76 76 76 76 76 76 76 76 76 76 76 76 76 76 102 102 102 102 102 102 102 102 102 102 102 102 102 102 102 76 76 76 76 76 76 76 76 76 76 76 76 76 76 76 102 102 102 102 102 102 102 102 102 102 102 102 102 102 102 76 76 76 76 76 76 76 76 76 76 76 76 76 76 76 102 102 102 102 102 102 102 102 102 102 102 102 102 102 102 ";
			CPPUNIT_ASSERT_EQUAL_MESSAGE("Pixel matrix incorrect", str, image.PixelMatrixString());
		}
		
		private:
			void AssertHeaderMetaDataCorrect(
				BitmapImage image,
				std::string filename,
				FileType fileType,
				int fileSize,
				int pixelOffset,
				int headerSize,
				int width,
				int height,
				int colourPlanes,
				int bpp,
				int compressionMethod,
				int rowSize,
				int pixelMatrixSize
			)
			{
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Header size incorrect", headerSize, image.GetHeaderSize());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Filename incorrect", filename, image.GetFilename());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("File type incorrect", fileType, image.GetFileType());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("File size incorrect", fileSize, image.GetFileSize());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Pixel offset incorrect", pixelOffset, image.GetPixelOffset());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Width incorrect", width, image.GetWidth());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Height incorrect", height, image.GetHeight());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Colour planes incorrect", colourPlanes, image.GetColourPlanes());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("BPP incorrect", bpp, image.GetBpp());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Compression method incorrect", compressionMethod, image.GetCompressionMethod());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Row size incorrect", rowSize, image.GetRowSize());
				CPPUNIT_ASSERT_EQUAL_MESSAGE("Pixel matrix size incorrect", pixelMatrixSize, image.GetPixelMatrixSize());
			}
};

CPPUNIT_TEST_SUITE_REGISTRATION( BitmapImageTests );