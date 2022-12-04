/**
* Created Tues 22nd January 2019
*/

#include <cppunit/extensions/HelperMacros.h>

#include "PixelList.h"

class PixelListTests : public CppUnit::TestFixture {
	
	CPPUNIT_TEST_SUITE( PixelListTests );
	CPPUNIT_TEST_SUITE_END();

	public:
	
		~PixelListTests() {}
		void tearDown() {}
		void setUp() {}
		
		private:
};

CPPUNIT_TEST_SUITE_REGISTRATION( PixelListTests );