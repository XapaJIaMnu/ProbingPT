#include "helpers/huffmanish.hh"

int main(int argc, char* argv[]){
	Huffman huffmanclass(argv[1]); //initialize
	huffmanclass.assign_values();
	huffmanclass.produce_lookups();
	huffmanclass.serialize_maps("/tmp/maps/");

	util::FilePiece filein(argv[1]);

	line_text firstline = splitLine(filein.ReadLine());

	std::vector<unsigned int> encoding = huffmanclass.encode_line(firstline);

	for (std::vector<unsigned int>::iterator it = encoding.begin(); it != encoding.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::map<unsigned int, std::string> lookup_target_phrase = huffmanclass.get_target_lookup_map();
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all1 = huffmanclass.get_word_all1_lookup_map();
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all2 = huffmanclass.get_word_all2_lookup_map();

	HuffmanDecoder decoder(&lookup_target_phrase, &lookup_word_all1, &lookup_word_all2);

	target_text tmp = decoder.decode_line(encoding);
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;
}
