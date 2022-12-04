/**
* Created Sat 13th October 2018
*/

#include <map>
#include <string>
#include <vector>

class BitmapImage;

class MosaicImage {

    typedef std::map<std::string, std::vector<std::string>> PixelImageList;

    public:
        /**
         * Constructor
         * @param pixelImageDir The directory containing the pixel images
         */
        MosaicImage(std::string pixelImageDir);

        /**
         * 
         */
        std::string PixelImageListToString();

        /**
         * 
         */
        void CreateMosaicImage(BitmapImage image);

    private:
        /**
         * 
         */
        void ReadPixelListFile();

        /**
         * 
         */
        std::string FindClosestValue(const std::string& value);

        /**
         * 
         */
        void CreateImageFile(std::string imageFileName, int height, int width, std::vector<int> pixelMatrix);

        PixelImageList m_pixelImageList;
        std::string m_pixelImageDir;
};