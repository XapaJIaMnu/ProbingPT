//Huffman encodes a line and also produces the vocabulary ids
#include "line_splitter.hh"
#include "vocabid.hh"

//Sorting for the second
struct sort_pair {
	bool operator()(const std::pair<std::string, unsigned int> &left, const std::pair<std::string, unsigned int> &right) {
		return left.second < right.second;
	}
};

class Huffman {
	unsigned long uniq_lines = 0; //Unique lines in the file.

	//Containers used when counting the occurence of a given phrase
	std::map<std::string, unsigned int> target_phrase_words;
	std::map<std::string, unsigned int> probabilities;
	std::map<std::string, unsigned int> word_all1;
	std::map<std::string, unsigned int> word_all2;

	//Same containers as vectors, for sorting
	std::vector<std::pair<std::string, unsigned int> > target_phrase_words_counts;
	std::vector<std::pair<std::string, unsigned int> > probabilities_counts;
	std::vector<std::pair<std::string, unsigned int> > word_all1_counts;
	std::vector<std::pair<std::string, unsigned int> > word_all2_counts;

	//Huffman maps
	std::map<std::string, unsigned short> target_phrase_huffman;
	std::map<std::string, unsigned short> probabilities_huffman;
	std::map<std::string, unsigned short> word_all1_huffman;
	std::map<std::string, unsigned short> word_all2_huffman;

	public:
		Huffman (const char *);
		void count_elements (line_text line);
		void assign_values();
		void serialize_maps();
};

