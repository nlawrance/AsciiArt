cmake_minimum_required(VERSION 3.0.0)
project(AsciiArt VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR})
add_executable(AsciiArt main.cpp BitmapImage.cpp enums.cpp MosaicImage.cpp PixelList.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
