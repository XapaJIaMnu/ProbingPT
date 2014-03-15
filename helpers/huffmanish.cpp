#include "huffmanish.hh"

Huffman::Huffman (const char * filepath) {
	//Read the file
	util::FilePiece filein(filepath);
	uniq_lines = 0;

	line_text prev_line; //Check for unique lines.

	while (true){
		line_text new_line;

		try {
			//Process line read
			new_line = splitLine(filein.ReadLine());
			count_elements(new_line); //Counts the number of elements, adds new and increments counters.

		} catch (util::EndOfFileException e){
			std::cerr << "Unique entries counted: ";
			break;
		}

		if (new_line.source_phrase == prev_line.source_phrase){
			continue;
		} else {
			uniq_lines++;
			prev_line = new_line;
		}
	}

	std::cerr << uniq_lines << std::endl;
}

void Huffman::count_elements(line_text linein){
	//For target phrase:
	util::TokenIter<util::SingleCharacter> it(linein.target_phrase, util::SingleCharacter(' '));
	while (it) {
		//Check if we have that entry
		std::map<std::string, unsigned int>::iterator mapiter;
		target_phrase_words.find(it->as_string());

		if (mapiter != target_phrase_words.end()){
			//If the element is found, increment the count.
			mapiter->second++;
		} else {
			//Else create a new entry;
			target_phrase_words.insert(std::pair<std::string, unsigned int>(it->as_string(), 1));
		}
	}

	//For probabilities

	util::TokenIter<util::SingleCharacter> probit(linein.prob, util::SingleCharacter(' '));
	while (probit) {
		//Check if we have that entry
		std::map<std::string, unsigned int>::iterator mapiter2;
		probabilities.find(probit->as_string());

		if (mapiter2 != probabilities.end()){
			//If the element is found, increment the count.
			mapiter2->second++;
		} else {
			//Else create a new entry;
			probabilities.insert(std::pair<std::string, unsigned int>(probit->as_string(), 1));
		}
	}

	//For word allignment 1
	std::map<std::string, unsigned int>::iterator mapiter3;
	word_all1.find(linein.word_all1.as_string());

	if (mapiter3 != word_all1.end()){
		//If the element is found, increment the count.
		mapiter3->second++;
	} else {
		//Else create a new entry;
		word_all1.insert(std::pair<std::string, unsigned int>(linein.word_all1.as_string(), 1));
	}

	//For word allignment 2
	std::map<std::string, unsigned int>::iterator mapiter4;
	word_all2.find(linein.word_all2.as_string());

	if (mapiter4 != word_all2.end()){
		//If the element is found, increment the count.
		mapiter4->second++;
	} else {
		//Else create a new entry;
		word_all2.insert(std::pair<std::string, unsigned int>(linein.word_all2.as_string(), 1));
	}

}

//Assigns huffman values for each unique element
void Huffman::assign_values() {
	//First create vectors for all maps so that we could sort them later.


	//Create a vector for target phrases
	for(std::map<std::string, unsigned int>::iterator it = target_phrase_words.begin(); it != target_phrase_words.end(); it++ ) {
		target_phrase_words_counts.push_back(*it);
	}
	//Sort it
	std::sort(target_phrase_words_counts.begin(), target_phrase_words_counts.end(), sort_pair());

	//Create a vector for probabilities
	for(std::map<std::string, unsigned int>::iterator it = probabilities.begin(); it != probabilities.end(); it++ ) {
		probabilities_counts.push_back(*it);
	}
	//Sort it
	std::sort(probabilities_counts.begin(), probabilities_counts.end(), sort_pair());

	//Create a vector for word allignments 1
	for(std::map<std::string, unsigned int>::iterator it = word_all1.begin(); it != word_all1.end(); it++ ) {
		word_all1_counts.push_back(*it);
	}
	//Sort it
	std::sort(word_all1_counts.begin(), word_all1_counts.end(), sort_pair());

	//Create a vector for word allignments 2
	for(std::map<std::string, unsigned int>::iterator it = word_all2.begin(); it != word_all2.end(); it++ ) {
		word_all2_counts.push_back(*it);
	}
	//Sort it
	std::sort(word_all2_counts.begin(), word_all2_counts.end(), sort_pair());


	//Afterwards we assign a value for each phrase, starting from 1, as zero is reserved for delimiter
	unsigned short i = 1; //huffman code
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = target_phrase_words_counts.begin();
	 it != target_phrase_words_counts.end(); it++){
		target_phrase_huffman.insert(std::pair<std::string, unsigned short>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for target prases is: " << i << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = probabilities_counts.begin();
	 it != probabilities_counts.end(); it++){
		probabilities_huffman.insert(std::pair<std::string, unsigned short>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for probabilities is: " << i << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = word_all1_counts.begin();
	 it != word_all1_counts.end(); it++){
		word_all1_huffman.insert(std::pair<std::string, unsigned short>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for word all1 is: " << i << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = word_all2_counts.begin();
	 it != word_all2_counts.end(); it++){
		word_all2_huffman.insert(std::pair<std::string, unsigned short>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for word all2 is: " << i << std::endl;
}

void Huffman::serialize_maps(){
	std::cerr << "STUB!" << std::endl;
}