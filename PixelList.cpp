/**
* Created Tues 22nd January 2019
*/

#include "PixelList.h"

#include "BitmapImage.h"

#include <cstring>
#include <dirent.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>

PixelList::PixelList() {}

void PixelList::CreatePixelImageListFile(std::string pDir)
{
	DIR *dir;
    struct dirent *dent;
    // open the directory containing the pixel images
    dir = opendir(pDir.c_str());
    std::map<int, std::vector<std::string>> tmpPixelImageList;
    
    if (dir != NULL) {
        while ((dent=readdir(dir)) != NULL) {
        	if (*(dent->d_name) != '.') {
        	
        		// Is it a file or a directory?
        		bool isFile = false;
        		// Parse the directory name for a '.'
        		for (char *tmp = dent->d_name; *tmp != '\0'; tmp++) {
        			if (strcmp(tmp, ".bmp") == 0) {
        				isFile = true;
        			}
        		}
        		
        		// If it is a file
				if (isFile) {
                    try
                    {
                        BitmapImage image(pDir + dent->d_name);
                        image.ReadPixelMarix();
                        int greyscale = image.GetAverageGreyScaleValue();

                        if (tmpPixelImageList.find(greyscale) != tmpPixelImageList.end())
                        {
                            tmpPixelImageList[greyscale].push_back(dent->d_name);
                        }
                        else
                        {
                            tmpPixelImageList[greyscale] = std::vector<std::string> {dent->d_name};
                        }
                    }
                    catch (std::exception& e)
                    {
                        std::cerr << "Error reading " << pDir + dent->d_name << "\n";
                        std::cerr << e.what() << "\n";
                    }
				}
			}
        }
    }
    closedir(dir);

    std::string listFilename = pDir + "pixelList.txt";
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