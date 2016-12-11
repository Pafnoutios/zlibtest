/**
 * \author John Szwast
 */


#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include "comdec_zlib.h"


using namespace std::literals;


struct filepair
{
  std::string sourcefilename;
  std::string compressedfilename;
  std::string decompressedfilename;
};


std::array<filepair const, 3> const FilePairs =
{
  filepair{ "source-kjv.txt"s, "compressed-kjv.z"s, "dest-kjv.txt"s },
  filepair{ "source-nile.jpg"s, "compressed-nile.z"s, "dest-nile.jpg"s },
  filepair{ "source-lena.tiff"s, "compressed-lena.z"s, "dest-lena.tiff"s }
};



int main()
{
  ComdecZlib zlibber;

  for (auto& file : FilePairs)
  {
    zlibber.compress(file.sourcefilename, file.compressedfilename);
    zlibber.decompress(file.compressedfilename, file.decompressedfilename);
    zlibber.compare(file.sourcefilename, file.decompressedfilename);
  }
}

