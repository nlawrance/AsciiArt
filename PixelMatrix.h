/**
 * @author Nicholas Lawrance
 * @date 2022-12-04
 * @copyright Copyright (c) 2022
 */

#include <string>
#include <vector>

/**
 * @brief 
 * 
 */
struct Pixel
{
    Pixel(unsigned int red, unsigned int green, unsigned int blue);

    Pixel(unsigned int grey);

    unsigned int Red;
    unsigned int Green;
    unsigned int Blue;
    unsigned int GreyScale;
};

/**
 * @brief 
 * 
 */
class PixelMatrix
{
public:
    PixelMatrix(unsigned int height, unsigned int width);
    ~PixelMatrix() = default;

    /**
     * @brief Set the Pixel At Position object
     * 
     * @param pixel 
     * @param x 
     * @param y 
     */
    void SetPixelAtPosition(const Pixel& pixel, unsigned int x, unsigned int y);

    /**
     * @brief
     * 
     * @return double 
     */
    double GetAverageGreyScaleValue() const;

    /**
     * @brief 
     * 
     * @param x 
     * @param y 
     * @return int 
     */
    unsigned int GetGreyScalePixelValue(unsigned int x, unsigned int y) const;

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string ToString() const;
 
private:
    /**
     * @brief Set the Pixel At Position object
     * 
     * @param pixel 
     * @param x 
     * @param y 
     */
    Pixel& GetPixelAtPosition(unsigned int x, unsigned int y);
    const Pixel& GetPixelAtPosition(unsigned int x, unsigned int y) const;

    unsigned int m_height = 0;
    unsigned int m_width = 0;
    std::vector<Pixel> m_pixelMatrix;
};