#include "../util/string_piece.hh"
#include "../util/tokenize_piece.hh"
#include "../util/file_piece.hh"
#include <vector>
#include <stdlib.h> //atof
#include "vocabid.hh"

//Struct for holding processed line
struct line_text {
	StringPiece source_phrase;
	StringPiece target_phrase;
	StringPiece prob;
	StringPiece word_all1;
	StringPiece word_all2;
};

//Struct for holding processed line
struct target_text {
	std::vector<unsigned int> target_phrase;
	std::vector<float> prob;
	std::vector<int> word_all1;
	std::vector<int> word_all2;
};

//Appends to the vector used for outputting.
std::pair<std::vector<char>::iterator, int> vector_append(line_text* input, std::vector<char>* outputvec, std::vector<char>::iterator it, bool new_entry);

//Find maximum number of lines
unsigned long getUniqLines(const char * filepath);

//Ask if it's better to have it receive a pointer to a line_text struct
line_text splitLine(StringPiece textin);

//Should be no problem returning a vector http://stackoverflow.com/questions/3721217/returning-a-c-stdvector-without-a-copy
std::vector<target_text> splitTargetLine(StringPiece textin);

target_text splitSingleTargetLine(StringPiece textin);

std::vector<float> splitProbabilities(StringPiece textin);

std::vector<int> splitWordAll1(StringPiece textin);

std::vector<int> splitWordAll2(StringPiece textin);
