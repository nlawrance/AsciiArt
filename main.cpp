/**
*    Created by Nicholas Lawrance 10th June 2018
*/

#include "BitmapImage.h"
#include "MosaicImage.h"
#include "PixelList.h"

#include <iostream>
#include <string.h>
#include <stdio.h>

int     opterr = 1,             /* if error message should be printed */
        optind = 1,             /* index into parent argv vector */
        optopt,                 /* character checked for validity */
        optreset;               /* reset getopt */
char    *optarg;                /* argument associated with option */

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

/*
 * getopt --
 *      Parse argc/argv argument vector.
 */
int getopt(int nargc, char * const nargv[], const char *ostr)
{
  static char *place = EMSG;              /* option letter processing */
  const char *oli;                              /* option letter list index */

  if (optreset || !*place) {              /* update scanning pointer */
    optreset = 0;
    if (optind >= nargc || *(place = nargv[optind]) != '-') {
      place = EMSG;
      return (-1);
    }
    if (place[1] && *++place == '-') {      /* found "--" */
      ++optind;
      place = EMSG;
      return (-1);
    }
  }                                       /* option letter okay? */
  if ((optopt = (int)*place++) == (int)':' ||
    !(oli = strchr(ostr, optopt))) {
      /*
      * if the user didn't specify '-' as an option,
      * assume it means -1.
      */
      if (optopt == (int)'-')
        return (-1);
      if (!*place)
        ++optind;
      if (opterr && *ostr != ':')
        (void)printf("illegal option -- %c\n", optopt);
      return (BADCH);
  }
  if (*++oli != ':') {                    /* don't need argument */
    optarg = NULL;
    if (!*place)
      ++optind;
  }
  else {                                  /* need an argument */
    if (*place)                     /* no white space */
      optarg = place;
    else if (nargc <= ++optind) {   /* no arg */
      place = EMSG;
      if (*ostr == ':')
        return (BADARG);
      if (opterr)
        (void)printf("option requires an argument -- %c\n", optopt);
      return (BADCH);
    }
    else                            /* white space */
      optarg = nargv[optind];
    place = EMSG;
    ++optind;
  }
  return (optopt);                        /* dump back option letter */
}

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

    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
        switch (opt)
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
                mosaicPixelDirectory = strdup(optarg);
                break;
            default:
                throw std::invalid_argument("Invalid option.");
        }
    }
    
    argc -= optind;
    argv += optind;       
    

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