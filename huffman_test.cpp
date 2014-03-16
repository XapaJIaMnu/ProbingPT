#include "helpers/huffmanish.hh"

int main(int argc, char* argv[]){
	Huffman huffmanclass(argv[1]); //initialize
	huffmanclass.assign_values();
	huffmanclass.produce_lookups();

	util::FilePiece filein(argv[1]);

	line_text firstline = splitLine(filein.ReadLine());

	std::vector<unsigned int> encoding = huffmanclass.encode_line(firstline);

	for (std::vector<unsigned int>::iterator it = encoding.begin(); it != encoding.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::map<unsigned int, std::string> lookup_target_phrase = huffmanclass.get_target_lookup_map();
	std::map<unsigned int, std::string> lookup_probabilities = huffmanclass.get_probabilities_lookup_map();
	std::map<unsigned int, std::string> lookup_word_all1 = huffmanclass.get_word_all1_lookup_map();
	std::map<unsigned int, std::string> lookup_word_all2 = huffmanclass.get_word_all2_lookup_map();

	HuffmanDecoder decoder(&lookup_target_phrase, &lookup_probabilities,
	 &lookup_word_all1, &lookup_word_all2);

	decoder.decode_line(encoding);
}