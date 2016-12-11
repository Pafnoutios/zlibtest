/**
* \author John Szwast
*/


#include "comdec.h"
#include <fstream>


namespace
{
  template<class Func>
  std::chrono::high_resolution_clock::duration time(Func f)
  {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();

    return end - start;
  }


  bool equal(std::istream& one, std::istream& two)
  {
    std::istreambuf_iterator<char> one_it(one);
    std::istreambuf_iterator<char> end;
    std::istreambuf_iterator<char> two_it(two);

    return std::equal(one_it, end, two_it, end);
  }
}


void Comdec::compress(std::string sourcename, std::string destname)
{
  timed_process(sourcename, destname, &Comdec::do_compress, "  Compressing");
}


void Comdec::decompress(std::string sourcename, std::string destname)
{
  timed_process(sourcename, destname, &Comdec::do_decompress, "Decompressing");
}


void Comdec::compare(std::string onename, std::string twoname)
{
  std::ifstream onefile(onename, std::ios::binary);
  std::ifstream twofile(twoname, std::ios::binary);
  std::cout << (equal(onefile, twofile) ? "Files match.\n" : "Files DO NOT match!\n");
}


void Comdec::timed_process(std::string sourcename,
  std::string destname,
  void (Comdec::* process)(std::istream &, std::ostream &),
  std::string processname)
{
  auto dur = process_file(sourcename, destname, process);
  log << processname << " " << sourcename << ": "
    << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
    << "ms\n";
}


std::chrono::high_resolution_clock::duration Comdec::process_file(
  std::string sourcename,
  std::string destname,
  void (Comdec::* process)(std::istream &, std::ostream &))
{
  std::ifstream infile(sourcename, std::ios::binary);
  std::ofstream outfile(destname, std::ios::binary);

  return time([&infile, &outfile, this, process]()
  {
    (this->*process)(infile, outfile);
  });
}


