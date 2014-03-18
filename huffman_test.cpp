#include "helpers/huffmanish.hh"

int main(int argc, char* argv[]){
	Huffman huffmanclass(argv[1]); //initialize
	huffmanclass.assign_values();
	huffmanclass.produce_lookups();
	huffmanclass.serialize_maps("/tmp/maps/");

	util::FilePiece filein(argv[1]);

	line_text firstline = splitLine(filein.ReadLine());
	line_text line2 = splitLine(filein.ReadLine());
	line_text line3 = splitLine(filein.ReadLine());
	line_text line4 = splitLine(filein.ReadLine());
	line_text line5 = splitLine(filein.ReadLine());
	line_text line6 = splitLine(filein.ReadLine());
	line_text line7 = splitLine(filein.ReadLine());
	line_text line8 = splitLine(filein.ReadLine());
	line_text line9 = splitLine(filein.ReadLine());
	line_text line10 = splitLine(filein.ReadLine());

	std::cout << "BEGIN! Encode 10 lines!" << std::endl;

	std::vector<unsigned char> encoding = huffmanclass.full_encode_line(firstline);
	std::vector<unsigned char> encoding2 = huffmanclass.full_encode_line(line2);
	std::vector<unsigned char> encoding3 = huffmanclass.full_encode_line(line3);
	std::vector<unsigned char> encoding4 = huffmanclass.full_encode_line(line4);
	std::vector<unsigned char> encoding5 = huffmanclass.full_encode_line(line5);
	std::vector<unsigned char> encoding6 = huffmanclass.full_encode_line(line6);
	std::vector<unsigned char> encoding7 = huffmanclass.full_encode_line(line7);
	std::vector<unsigned char> encoding8 = huffmanclass.full_encode_line(line8);
	std::vector<unsigned char> encoding9 = huffmanclass.full_encode_line(line9);
	std::vector<unsigned char> encoding10 = huffmanclass.full_encode_line(line10);

	std::cout << "END! Encoded 10 lines!" << std::endl;
	/*
	for (std::vector<unsigned char>::iterator it = encoding.begin(); it != encoding.end(); it++){
		std::cout << (int)*it << " ";
	}
	std::cout << std::endl;
	*/
	std::cout <<"BEGIN! Initialize decoder" <<std::endl;
	std::map<unsigned int, std::string> lookup_target_phrase = huffmanclass.get_target_lookup_map();
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all1 = huffmanclass.get_word_all1_lookup_map();
	std::map<unsigned int, std::vector<unsigned char> > lookup_word_all2 = huffmanclass.get_word_all2_lookup_map();

	HuffmanDecoder decoder(&lookup_target_phrase, &lookup_word_all1, &lookup_word_all2);
	std::cout <<"END! Initialized decoder" <<std::endl;

	target_text tmp = (decoder.full_decode_line(encoding))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding2))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding3))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding4))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding5))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding6))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding7))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding8))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding9))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	tmp = (decoder.full_decode_line(encoding10))[0];
	std::cout << decoder.getTargetWordsFromIDs(tmp.target_phrase) << " ";

	for (std::vector<float>::iterator it = tmp.prob.begin(); it != tmp.prob.end(); it++){
		std::cout << *it << " ";
	}

	std::cout << (int)tmp.word_all1[0] << " " << (int)tmp.word_all2[0] << std::endl;

	std::vector<unsigned int> test = {1};
	std::cout << "Size is! " << (vbyte_encode_line(test)).size() << std::endl;

	std::vector<unsigned int> test2 = {1,2};
	std::cout << "Size is! " << (vbyte_encode_line(test2)).size() << std::endl;

	std::vector<unsigned int> test3 = {250430};
	std::cout << "Size is! " << (vbyte_encode_line(test3)).size() << std::endl;

}
