#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"
#include "util/tokenize_piece.hh"
#include "util/murmur_hash.hh"
#include "util/probing_hash_table.hh"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>

int main() {

  std::fstream backing("../source.uniq.rand.gz", std::ios::in);
  util::FilePiece test(backing, NULL, 1);
  //An example of how not to read a file (try/catch), will change later, but for now, hacking
  while(true){
    try {
      StringPiece line = test.ReadLine();

      std::cout << line << std::endl;
     
    } catch (util::EndOfFileException e){
      std::cout << "End of file" << std::endl;
      break;
    }
  }

  return 0;
}