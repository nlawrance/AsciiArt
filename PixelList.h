/**
* Created Tues 22nd January 2019
*/

#include <string>

class PixelList
{
    public:
        PixelList();

        /**
         * 
         * @param directoryPath
         */
        void CreatePixelImageListFile(const std::string& directoryPath);
};