//Huffman encodes a line and also produces the vocabulary ids
#include "line_splitter.hh"
#include "vocabid.hh"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

//Sorting for the second
struct sort_pair {
	bool operator()(const std::pair<std::string, unsigned int> &left, const std::pair<std::string, unsigned int> &right) {
		return left.second > right.second; //This puts biggest numbers first.
	}
};

struct sort_pair_vec {
	bool operator()(const std::pair<std::vector<unsigned char>, unsigned int> &left, const std::pair<std::vector<unsigned char>, unsigned int> &right) {
		return left.second > right.second; //This puts biggest numbers first.
	}
};

class Huffman {
	unsigned long uniq_lines = 0; //Unique lines in the file.

	//Containers used when counting the occurence of a given phrase
	std::map<std::string, unsigned int> target_phrase_words;
	std::map<std::vector<unsigned char>, unsigned int> word_all1;
	std::map<std::vector<unsigned char>, unsigned int> word_all2;

	//Same containers as vectors, for sorting
	std::vector<std::pair<std::string, unsigned int> > target_phrase_words_counts;
	std::vector<std::pair<std::vector<unsigned char>, unsigned int> > word_all1_counts;
	std::vector<std::pair<std::vector<unsigned char>, unsigned int> > word_all2_counts;

	//Huffman maps
	std::map<std::string, unsigned int> target_phrase_huffman;
	std::map<std::vector<unsigned char>, unsigned int> word_all1_huffman;
	std::map<std::vector<unsigned char>, unsigned int> word_all2_huffman;

	//inverted maps
	std::map<unsigned int, std::string> lookup_target_phrase;
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all1;
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all2;

	public:
		Huffman (const char *);
		void count_elements (line_text line);
		void assign_values();
		void serialize_maps(const char * dirname);
		void produce_lookups();

		std::vector<unsigned int> encode_line(line_text line);

		//Getters
		const std::map<unsigned int, std::string> get_target_lookup_map() const{
			return lookup_target_phrase;
		}
		const std::map<unsigned int, std::vector<unsigned char> > get_word_all1_lookup_map() const{
			return lookup_word_all1;
		}
		const std::map<unsigned int, std::vector<unsigned char> > get_word_all2_lookup_map() const{
			return lookup_word_all2;
		}
};

class HuffmanDecoder {
	std::map<unsigned int, std::string> lookup_target_phrase;
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all1;
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all2;

public:
	HuffmanDecoder (const char *);
	HuffmanDecoder (std::map<unsigned int, std::string> *,
	 std::map<unsigned int, std::vector<unsigned char> > *, std::map<unsigned int, std::vector<unsigned char> > *);

	//Getters
	const std::map<unsigned int, std::string> get_target_lookup_map() const{
		return lookup_target_phrase;
	}
	const std::map<unsigned int, std::vector<unsigned char> > get_word_all1_lookup_map() const{
		return lookup_word_all1;
	}
	const std::map<unsigned int, std::vector<unsigned char> > get_word_all2_lookup_map() const{
		return lookup_word_all2;
	}

	inline std::string getTargetWordFromID(unsigned int id);

	std::string getTargetWordsFromIDs(std::vector<unsigned int> ids);

	target_text decode_line (std::vector<unsigned int> input);
};

std::string getTargetWordsFromIDs(std::vector<unsigned int> ids, std::map<unsigned int, std::string> * lookup_target_phrase);

inline std::string getTargetWordFromID(unsigned int id, std::map<unsigned int, std::string> * lookup_target_phrase);

inline unsigned int reinterpret_float(float * num);

inline float reinterpret_uint(unsigned int * num);

std::vector<unsigned char> vbyte_encode_line(std::vector<unsigned int> line);
std::vector<unsigned char> vbyte_encode(unsigned int num);
std::vector<unsigned int> vbyte_decode(std::vector<unsigned char> line);
unsigned int bytes_to_int(std::vector<unsigned char> number);
