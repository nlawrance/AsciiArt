/**
* Created Tues 22nd January 2019
*/

#include "PixelList.h"

#include "BitmapImage.h"

#include <cstring>
#include <exception>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>

PixelList::PixelList() {}

void PixelList::CreatePixelImageListFile(const std::string& directoryPath)
{
    std::map<int, std::vector<std::string>> tmpPixelImageList;
    for (const auto& entry : std::filesystem::directory_iterator{directoryPath})
    {
        // If it is a file
        if (!entry.is_directory()) {
            const std::string path = entry.path().generic_string();
            try
            {
                BitmapImage image(path);
                image.ReadPixelMarix();
                int greyscale = image.GetAverageGreyScaleValue();

                if (tmpPixelImageList.find(greyscale) != tmpPixelImageList.end())
                {
                    tmpPixelImageList[greyscale].push_back(path);
                }
                else
                {
                    tmpPixelImageList[greyscale] = std::vector<std::string> {path};
                }
            }
            catch (std::exception& e)
            {
                std::cerr << "Error reading " << path << "\n";
                std::cerr << e.what() << "\n";
            }
        }
        std::cout << entry.path() << std::endl;
    }

    std::string listFilename = directoryPath + "pixelList.txt";
    std::ofstream pixelListFile;
    pixelListFile.open(listFilename);

    for (auto pair : tmpPixelImageList)
    {
        pixelListFile << pair.first;
        for (auto filename : pair.second)
        {
            pixelListFile << "\t" << filename;
        }
        pixelListFile << "\n";
    }

    pixelListFile.close();
}