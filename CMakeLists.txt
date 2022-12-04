cmake_minimum_required(VERSION 3.12.0)
project(AsciiArt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

include(CTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR})
add_executable(AsciiArt main.cpp BitmapImage.cpp enums.cpp MosaicImage.cpp PixelList.cpp PixelMatrix.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
