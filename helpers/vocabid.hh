//Serialization
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include <map> //Container
#include "hash.hh" //Hash of elements

#include "util/string_piece.hh"  //Tokenization and work with StringPiece
#include "util/tokenize_piece.hh"

void add_to_map(std::map<uint64_t, std::string> *karta, StringPiece textin);

void serialize_map(std::map<uint64_t, std::string> *karta, char* filename);

void read_map(std::map<uint64_t, std::string> *karta, char* filename);

// If necessary those methods could use StringPiece instead of std::string, I don't
// know which is better.
std::string getStringFromID(std::map<uint64_t, std::string> *karta, uint64_t id);

std::string getStringFromIDs(std::map<uint64_t, std::string> *karta, std::vector<uint64_t> ids);

uint64_t getVocabID(std::string candidate);

//Get vocab iDs from a whole target fraze.
std::vector<uint64_t> getVocabIDs(StringPiece textin);