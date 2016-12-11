#pragma once

/**
* \author John Szwast
*/


#include <chrono>
#include <iostream>
#include <string>


class Comdec
{
public:
  void compress(std::string sourcename, std::string destname);
  void decompress(std::string sourcename, std::string destname);
  void compare(std::string onename, std::string twoname);

private:
  virtual void do_compress(std::istream& in, std::ostream& out) = 0;
  virtual void do_decompress(std::istream& in, std::ostream& out) = 0;

  void timed_process(std::string sourcename,
    std::string destname,
    void (Comdec::*process) (std::istream&, std::ostream&),
    std::string processname);

  std::chrono::high_resolution_clock::duration process_file(
    std::string sourcename,
    std::string destname,
    void (Comdec::*process) (std::istream&, std::ostream&));

  std::ostream& log = std::cout;
};



