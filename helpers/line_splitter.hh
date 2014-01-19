#include "../util/string_piece.hh"
#include "../util/tokenize_piece.hh"
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
	std::vector<uint64_t> target_phrase;
	std::vector<double> prob;
	std::vector<int> word_all1;
	std::vector<int> word_all2;
};

//Ask if it's better to have it receive a pointer to a line_text struct
line_text splitLine(StringPiece textin);

//Should be no problem returning a vector http://stackoverflow.com/questions/3721217/returning-a-c-stdvector-without-a-copy
std::vector<target_text> splitTargetLine(StringPiece textin);

target_text splitSingleTargetLine(StringPiece textin);

std::vector<double> splitProbabilities(StringPiece textin);

std::vector<int> splitWordAll1(StringPiece textin);

std::vector<int> splitWordAll2(StringPiece textin);
