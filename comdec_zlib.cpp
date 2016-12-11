/**
* \author John Szwast
*/


#include "comdec_zlib.h"

#include <cstdint>
#include <vector>
#include "zlib.h"


void ComdecZlib::do_compress(std::istream & in, std::ostream & out)
{
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

void ComdecZlib::do_decompress(std::istream & in, std::ostream & out)
{
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
