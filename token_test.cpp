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
#include <boost/algorithm/string.hpp> // TRim string

int main() {

  StringPiece pesho = StringPiece("I like pie ||| 123154");
  StringPiece kiro = StringPiece("Kiro is a stupid brat ||| 12351");

  const char delim[] = "|||";
  StringPiece str;
  std::string temp; //Temp string for conversion
  int num;

  util::TokenIter<util::MultiCharacter> it(pesho, util::MultiCharacter(delim));
  str = *it;
  std::cout << str << std::endl;
  it++;
  //Convert to int
  str = *it;
  temp = (str.as_string().substr(1,std::string::npos));
  num = atoi(temp.c_str());
  //num = *it;
  std::cout << num << std::endl;

  util::TokenIter<util::MultiCharacter> it1(kiro, util::MultiCharacter(delim));
  str = *it1;
  std::cout << str << std::endl;
  it1++;
  //Convert to int
  str = *it1;
  temp = (str.as_string().substr(1,std::string::npos));
  num = atoi(temp.c_str());
  //num = *it1;
  std::cout << num << std::endl;

  return 0;
}