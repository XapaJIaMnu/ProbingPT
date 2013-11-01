 
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

  StringPiece tohash = StringPiece("Hash text2");
  StringPiece identical = StringPiece("Hash text2");
  std::size_t len = tohash.size();
  uint64_t key = util::MurmurHashNative(&tohash, len);
  std::size_t len2 = identical.size();
  uint64_t key2 = util::MurmurHashNative(&identical, len2);
  std::cout << "Hash 1 is " << key << " With len " << len << std::endl;
  std::cout << "Hash 2 is " << key2 << " With len " << len2 << std::endl;

  return 0;
}