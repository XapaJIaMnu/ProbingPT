//Huffman encodes a line and also produces the vocabulary ids
#include "line_splitter.hh"
#include "vocabid.hh"

//Sorting for the second
struct sort_pair {
	bool operator()(const std::pair<std::string, unsigned int> &left, const std::pair<std::string, unsigned int> &right) {
		return left.second > right.second; //This puts biggest numbers first.
	}
};

//Struct for holding processed line
struct target_text_huffman {
	std::vector<unsigned int> target_phrase;
	std::vector<double> prob;
	std::vector<int> word_all1;
	std::vector<int> word_all2;
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
	std::map<std::string, unsigned int> target_phrase_huffman;
	std::map<std::string, unsigned int> probabilities_huffman;
	std::map<std::string, unsigned int> word_all1_huffman;
	std::map<std::string, unsigned int> word_all2_huffman;

	//inverted maps
	std::map<unsigned int, std::string> lookup_target_phrase;
	std::map<unsigned int, std::string> lookup_probabilities;
	std::map<unsigned int, std::string> lookup_word_all1;
	std::map<unsigned int, std::string> lookup_word_all2;

	public:
		Huffman (const char *);
		void count_elements (line_text line);
		void assign_values();
		void serialize_maps();
		void produce_lookups();

		std::vector<unsigned int> encode_line(line_text line);
		const std::map<unsigned int, std::string> get_target_lookup_map() const{
			return lookup_target_phrase;
		}
		const std::map<unsigned int, std::string> get_probabilities_lookup_map() const{
			return lookup_probabilities;
		}
		const std::map<unsigned int, std::string> get_word_all1_lookup_map() const{
			return lookup_word_all1;
		}
		const std::map<unsigned int, std::string> get_word_all2_lookup_map() const{
			return lookup_word_all2;
		}
};

class HuffmanDecoder {
	std::map<unsigned int, std::string> lookup_target_phrase;
	std::map<unsigned int, std::string> lookup_probabilities;
	std::map<unsigned int, std::string> lookup_word_all1;
	std::map<unsigned int, std::string> lookup_word_all2;

public:
	HuffmanDecoder (const char *);
	HuffmanDecoder (std::map<unsigned int, std::string> *, std::map<unsigned int, std::string> *,
	 std::map<unsigned int, std::string> *, std::map<unsigned int, std::string> *);

	target_text_huffman decode_line (std::vector<unsigned int> input);
};