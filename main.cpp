/**
*    Created by Nicholas Lawrance 10th June 2018
*/

#include "BitmapImage.h"
#include "MosaicImage.h"
#include "PixelList.h"

#include <iostream>
#include <set>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    const char* OPTLIST = "adfim:";
    const std::string helpText = 
        "./AsciiArt [-adfi] [-m pDir] filename\n"
        "\t-a\t\tReturns the average colour of the given file\n"
        "\t-d\t\tAcivate debug logging\n"
        "\t-f\t\tGenerate pixel list file\n"
        "\t-i\t\tActivate info mode (disable creation of mosaic image)\n"
        "\t-m\tpDir\tDirectory to get pixel images from";

    bool getAverageColour = false;
    bool createPixelListFile = false;
    bool isDebug = false;
    bool info = false;
    std::string mosaicPixelDirectory = "";

    // Ignore the program name.
    argc--;
    argv++;

    // Read the optional arguments.
    while (argc > 0)
    {
        if ((*argv)[0] != '-')
        {
            // Found all the optional arguments.
            break;
        }
        else
        {
            bool skipNext = false;
            for (int i = 1; (*argv)[i] != '\0'; i++)
            {
                switch ((*argv)[i])
                {
                    case 'a':
                        getAverageColour = true;
                        break;
                    case 'd':
                        isDebug = true;
                        break;
                    case 'f':
                        createPixelListFile = true;
                        break;
                    case 'i':
                        info = true;
                        break;
                    case 'm':
                        mosaicPixelDirectory = strdup(*(argv + 1));
                        skipNext = true;
                        break;
                    default:
                        throw std::invalid_argument("Invalid option.");
                }
                if (skipNext)
                {
                    argc--;
                    argv++;
                }
            }
        }

        // Move to the next argument.
        argc--;
        argv++;
    }      
    

    if (createPixelListFile)
    {
        PixelList list = PixelList();
        list.CreatePixelImageListFile(mosaicPixelDirectory);
        exit(EXIT_SUCCESS);
    }

    if (argc <= 0) {
        std::cerr << helpText << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string filename = *argv; 
  
    BitmapImage image = BitmapImage(filename);
    if (isDebug)
    {
        std::cout << image.HeaderString();
    }
    image.ReadPixelMarix();
    if (getAverageColour)
    {
        std::cout << static_cast<int>(image.GetAverageGreyScaleValue()) << "_\n";
    }
  
    if (!info)
    {
        if (mosaicPixelDirectory.empty())
        {
            std::cout << image.PrintASCIIPixelMatrix(true);
        }
        else
        {
            MosaicImage mosaicImage(mosaicPixelDirectory);
            mosaicImage.CreateMosaicImage(image);
        }
    }
  
    return 0;
} 