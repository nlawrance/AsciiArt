/**
* Created Sat 13th October 2018
*/

#include <cppunit/extensions/HelperMacros.h>

#include "MosaicImage.h"

class MosaicImageTests : public CppUnit::TestFixture {
	
	CPPUNIT_TEST_SUITE( MosaicImageTests );
	CPPUNIT_TEST( TestCreateMosaicImage );
	CPPUNIT_TEST_SUITE_END();

	public:
	
		~MosaicImageTests() {}
		void tearDown() {}
		void setUp() {}
		
		void TestCreateMosaicImage() {
			std::string pixelImageDir = "test_images/pixel_images";
			MosaicImage image = MosaicImage(pixelImageDir);
		}
		
		private:
};

CPPUNIT_TEST_SUITE_REGISTRATION( MosaicImageTests );