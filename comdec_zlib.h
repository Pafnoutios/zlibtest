#pragma once

/**
* \author John Szwast
*/


#include "comdec.h"


class ComdecZlib : public Comdec
{
private:
  static std::size_t const bufsize = 4 * 1024;

  virtual void do_compress(std::istream& in, std::ostream& out) override;
  virtual void do_decompress(std::istream& in, std::ostream& out) override;
};

