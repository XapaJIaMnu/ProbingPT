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

struct line_text {
  int value;
  StringPiece text;
};

line_text split_line(StringPiece textin);

line_text split_line(StringPiece textin) {
  const char delim[] = " ||| ";
  StringPiece str; //Temp string container
  std::string temp; //Temp string for conversion
  int num;
  line_text output;

  //Tokenize
  util::TokenIter<util::MultiCharacter> it(textin, util::MultiCharacter(delim));
  //Get string
  str = *it;
  output.text = str;
  it++;

  //Get num
  str = *it;

  //Convert to int
  temp = str.as_string();
  num = atoi(temp.c_str());
  output.value = num;

  return output;
}

int main() {

  StringPiece pesho = StringPiece("I like pie ||| 123154");
  StringPiece kiro = StringPiece("Kiro is a stupid brat ||| 12351");

  line_text tmp;

  tmp = split_line(kiro);

  std::cout << tmp.text << " " << tmp.value << std::endl;

  return 0;
}