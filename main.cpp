/**
 * \author John Szwast
 */


#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "zlib.h"


using namespace std::literals;


struct filepair
{
  std::string sourcefilename;
  std::string compressedfilename;
  std::string decompressedfilename;
};


std::array<filepair const, 1> const FilePairs =
{
  { "source-kjv.txt"s, "compressed-kjv.z"s, "dest-kjv.txt"s }
};


template<class Func>
std::chrono::high_resolution_clock::duration time(Func f)
{
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();

  return end - start;
}


void compress_zlib(std::istream& in, std::ostream& out)
{
  static std::size_t const bufsize = 4 * 1024;

  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = nullptr;

  std::vector<std::uint8_t> inbuf(bufsize);
  std::vector<std::uint8_t> outbuf(bufsize);

  stream.avail_in = 0;
  stream.avail_out = bufsize;
  stream.next_out = &*outbuf.begin();
  for (int status = deflateInit(&stream, Z_BEST_COMPRESSION);
    status == Z_OK;
    status = deflate(&stream, stream.avail_in == 0 ? Z_FINISH : Z_NO_FLUSH))
  {
    if (stream.avail_in == 0)
    {
      stream.next_in = &*inbuf.begin();
      stream.avail_in = in.read(reinterpret_cast<char*>(&*inbuf.begin()), bufsize).gcount();
    }
    if (stream.avail_out == 0)
    {
      out.write(reinterpret_cast<char const*>(&*outbuf.begin()), bufsize);
      stream.next_out = &*outbuf.begin();
      stream.avail_out = bufsize;
    }
  }

  out.write(reinterpret_cast<char const*>(&*outbuf.begin()), bufsize - stream.avail_out);
  deflateEnd(&stream);
}


void decompress_zlib(std::istream& in, std::ostream& out)
{
  static std::size_t const bufsize = 4 * 1024;

  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = nullptr;

  std::vector<std::uint8_t> inbuf(bufsize);
  std::vector<std::uint8_t> outbuf(bufsize);

  stream.avail_in = 0;
  stream.avail_out = bufsize;
  stream.next_out = &*outbuf.begin();
  for (int status = inflateInit(&stream);
    status == Z_OK;
    status = inflate(&stream, stream.avail_in == 0 ? Z_FINISH : Z_NO_FLUSH))
  {
    if (stream.avail_in == 0)
    {
      stream.next_in = &*inbuf.begin();
      stream.avail_in = in.read(reinterpret_cast<char*>(&*inbuf.begin()), bufsize).gcount();
    }
    if (stream.avail_out == 0)
    {
      out.write(reinterpret_cast<char const*>(&*outbuf.begin()), bufsize);
      stream.next_out = &*outbuf.begin();
      stream.avail_out = bufsize;
    }
  }

  out.write(reinterpret_cast<char const*>(&*outbuf.begin()), bufsize - stream.avail_out);
  inflateEnd(&stream);
}



int main()
{
  {
    std::ifstream infile(FilePairs[0].sourcefilename, std::ios::binary);
    std::ofstream outfile(FilePairs[0].compressedfilename, std::ios::binary);

    auto dur = time([&infile, &outfile]() {compress_zlib(infile, outfile); });
    std::cout << "  Compressing " << FilePairs[0].sourcefilename << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << "ms\n";
  }
  {
    std::ifstream infile(FilePairs[0].compressedfilename, std::ios::binary);
    std::ofstream outfile(FilePairs[0].decompressedfilename, std::ios::binary);

    auto dur = time([&infile, &outfile]() {decompress_zlib(infile, outfile); });
    std::cout << "Decompressing " << FilePairs[0].sourcefilename << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << "ms\n";
  }
}

