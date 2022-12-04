/**
 * @author Nicholas Lawrance
 * @date 2022-12-04
 * @copyright Copyright (c) 2022
 */

#include "PixelMatrix.h"

#include <numeric>
#include <sstream>

/**
 * @brief 
 * 
 * @param red 
 * @param green 
 * @param blue 
 * @return int 
 */
unsigned int RgbToGreyScale(unsigned int red, unsigned int green, unsigned int blue)
{
    constexpr static double RED_WEIGHT = 0.2126;
    constexpr static double GREEN_WEIGHT = 0.7152;
    constexpr static double BLUE_WEIGHT = 0.07222;
    return RED_WEIGHT * red + GREEN_WEIGHT * green + BLUE_WEIGHT * blue;
}

Pixel::Pixel(unsigned int red, unsigned int green, unsigned int blue)
    : Red(red)
    , Green(green)
    , Blue()
    , GreyScale(RgbToGreyScale(red, green, blue))
{}

Pixel::Pixel(unsigned int grey)
    : Red(grey)
    , Green(grey)
    , Blue(grey)
    , GreyScale(grey)
{}


PixelMatrix::PixelMatrix(unsigned int height, unsigned int width)
    : m_height(height)
    , m_width(width)
    , m_pixelMatrix(std::vector<Pixel>(height * width, { 0 }))
{}
   
void PixelMatrix::SetPixelAtPosition(const Pixel& pixel, unsigned int x, unsigned int y)
{
    GetPixelAtPosition(x, y) = pixel;
}

Pixel& PixelMatrix::GetPixelAtPosition(unsigned int x, unsigned int y)
{
    return m_pixelMatrix.at(x + m_width * y);
}

const Pixel& PixelMatrix::GetPixelAtPosition(unsigned int x, unsigned int y) const
{
    return m_pixelMatrix.at(x + m_width * y);
}

double PixelMatrix::GetAverageGreyScaleValue() const
{
    long total = std::accumulate(m_pixelMatrix.cbegin(), m_pixelMatrix.cend(), 0l,
        [](const Pixel& p1, const Pixel p2) {return p1.GreyScale + p2.GreyScale; } );
    return total / (m_height * m_width);
}

unsigned int PixelMatrix::GetGreyScalePixelValue(unsigned int x, unsigned int y) const
{
	return GetPixelAtPosition(x, y).GreyScale;
}

std::string PixelMatrix::ToString() const
{
	std::ostringstream oss;
	
	for (const auto& pixel : m_pixelMatrix)
	{
		oss << "(" << pixel.Red << ", " << pixel.Blue << ", " << pixel.Green << ") ";
	}
	
	return oss.str();
}
