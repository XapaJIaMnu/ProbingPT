 
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

  StringPiece tohash = StringPiece("Hash text");
  std::size_t len = tohash.size();
  uint64_t key = util::MurmurHashNative(&tohash, len);
  std::cout << "Hash is " << key << std::endl;

  return 0;
}