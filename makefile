#
#
#	Created by Nicholas Lawrance 24th Jan 2016
#
#

CC=g++
CFLAGS=-std=c++20 -Wall
OBJECTS=AsciiArt.o BitmapImage.o MosaicImage.o PixelList.o enums.o
EXECUTABLE=AsciiArt
LIBS=

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)
	
.cpp.o:
	$(CC) $(CFLAGS) -c $<
	
AsciiArt.o: BitmapImage.h MosaicImage.h 

BitmapImage.o: BitmapImage.h enums.h

MosaicImage.o: MosaicImage.h BitmapImage.h

PixelList.o: PixelList.h PixelList.h

enums.o: enums.h

tests:
	make -f makefile_tests

clean:
	rm *o $(EXECUTABLE) $(TEST_EXECUTABLE)

clean_tests:
	rm *o $(TEST_EXECUTABLE)

open:
	open -a TextWrangler *.h *.cpp makefile*
