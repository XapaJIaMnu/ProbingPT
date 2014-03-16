#include "huffmanish.hh"

Huffman::Huffman (const char * filepath) {
	//Read the file
	util::FilePiece filein(filepath);

	line_text prev_line; //Check for unique lines.
	int num_lines = 0 ;

	while (true){
		line_text new_line;

		num_lines++;

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
		mapiter = target_phrase_words.find(it->as_string());

		if (mapiter != target_phrase_words.end()){
			//If the element is found, increment the count.
			mapiter->second++;
		} else {
			//Else create a new entry;
			target_phrase_words.insert(std::pair<std::string, unsigned int>(it->as_string(), 1));
		}
		it++;
	}

	//For probabilities

	util::TokenIter<util::SingleCharacter> probit(linein.prob, util::SingleCharacter(' '));
	while (probit) {
		//Check if we have that entry
		std::map<std::string, unsigned int>::iterator mapiter2;
		mapiter2 = probabilities.find(probit->as_string());

		if (mapiter2 != probabilities.end()){
			//If the element is found, increment the count.
			mapiter2->second++;
		} else {
			//Else create a new entry;
			probabilities.insert(std::pair<std::string, unsigned int>(probit->as_string(), 1));
		}
		probit++;
	}

	//For word allignment 1
	std::map<std::string, unsigned int>::iterator mapiter3;
	mapiter3 = word_all1.find(linein.word_all1.as_string());

	if (mapiter3 != word_all1.end()){
		//If the element is found, increment the count.
		mapiter3->second++;
	} else {
		//Else create a new entry;
		word_all1.insert(std::pair<std::string, unsigned int>(linein.word_all1.as_string(), 1));
	}

	//For word allignment 2
	std::map<std::string, unsigned int>::iterator mapiter4;
	mapiter4 = word_all2.find(linein.word_all2.as_string());

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
	unsigned int i = 1; //huffman code
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = target_phrase_words_counts.begin();
	 it != target_phrase_words_counts.end(); it++){
		target_phrase_huffman.insert(std::pair<std::string, unsigned int>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for target prases is: " << i << std::endl;
	std::cerr << "Word 25 is: " << target_phrase_words_counts[25].first << " count is " << target_phrase_words_counts[25].second << std::endl;

	std::cerr << "prob 25 is: " << probabilities_counts[25].first << " count is " << probabilities_counts[25].second << std::endl;
	std::cerr << "word_all1 25 is: " << word_all1_counts[25].first << " count is " << word_all1_counts[25].second << std::endl;
	std::cerr << "word_all2 25 is: " << word_all2_counts[25].first << " count is " << word_all2_counts[25].second << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = probabilities_counts.begin();
	 it != probabilities_counts.end(); it++){
		probabilities_huffman.insert(std::pair<std::string, unsigned int>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for probabilities is: " << i << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = word_all1_counts.begin();
	 it != word_all1_counts.end(); it++){
		word_all1_huffman.insert(std::pair<std::string, unsigned int>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for word all1 is: " << i << std::endl;

	i = 1; //Reset i for the next map
	for(std::vector<std::pair<std::string, unsigned int> >::iterator it = word_all2_counts.begin();
	 it != word_all2_counts.end(); it++){
		word_all2_huffman.insert(std::pair<std::string, unsigned int>(it->first, i));
		i++; //Go to the next huffman code
	}
	std::cerr << "Maximum huffman code for word all2 is: " << i << std::endl;

	//After lookups are produced, clear some memory usage of objects not needed anymore.
	target_phrase_words.clear();
	probabilities.clear();
	word_all1.clear();
	word_all2.clear();

	target_phrase_words_counts.clear();
	probabilities_counts.clear();
	word_all1_counts.clear();
	word_all2_counts.clear();

}

void Huffman::serialize_maps(const char * dirname){
	//Note that directory name should exist.
	std::string basedir(dirname);
	std::string target_phrase_path(basedir + "/target_phrases");
	std::string probabilities_path(basedir + "/probs");
	std::string word_all1_path(basedir + "/Wall1");
	std::string word_all2_path(basedir + "/Wall2");

	//Target phrase
	std::ofstream os (target_phrase_path, std::ios::binary);
	boost::archive::text_oarchive oarch(os);
	oarch << lookup_target_phrase;
	os.close();

	//Target probabilities
	std::ofstream os1 (probabilities_path, std::ios::binary);
	boost::archive::text_oarchive oarch1(os1);
	oarch1 << lookup_probabilities;
	os1.close();

	//Word all1
	std::ofstream os2 (word_all1_path, std::ios::binary);
	boost::archive::text_oarchive oarch2(os2);
	oarch2 << lookup_word_all1;
	os2.close();

	//Word all2
	std::ofstream os3 (word_all2_path, std::ios::binary);
	boost::archive::text_oarchive oarch3(os3);
	oarch3 << lookup_word_all2;
	os3.close();
}

std::vector<unsigned int> Huffman::encode_line(line_text line){
	std::vector<unsigned int> retvector;

	//Get target_phrase first.
	util::TokenIter<util::SingleCharacter> it(line.target_phrase, util::SingleCharacter(' '));
	while (it) {
		retvector.push_back(target_phrase_huffman.find(it->as_string())->second);
		it++;
	}
	//Add a zero;
	retvector.push_back(0);

	//Get target_phrase first.
	util::TokenIter<util::SingleCharacter> probit(line.prob, util::SingleCharacter(' '));
	while (probit) {
		retvector.push_back(probabilities_huffman.find(probit->as_string())->second);
		probit++;
	}
	//Add a zero;
	retvector.push_back(0);

	//Get Word allignments
	retvector.push_back(word_all1_huffman.find(line.word_all1.as_string())->second);
	//No need of zero here, because we are expecting a single number only

	retvector.push_back(word_all2_huffman.find(line.word_all2.as_string())->second);
	retvector.push_back(0);

	return retvector;
}

void Huffman::produce_lookups(){
	//basically invert every map that we have
	for(std::map<std::string, unsigned int>::iterator it = target_phrase_huffman.begin(); it != target_phrase_huffman.end(); it++ ) {
		lookup_target_phrase.insert(std::pair<unsigned int, std::string>(it->second, it->first));
	}

	for(std::map<std::string, unsigned int>::iterator it = probabilities_huffman.begin(); it != probabilities_huffman.end(); it++ ) {
		lookup_probabilities.insert(std::pair<unsigned int, std::string>(it->second, it->first));
	}

	for(std::map<std::string, unsigned int>::iterator it = word_all1_huffman.begin(); it != word_all1_huffman.end(); it++ ) {
		lookup_word_all1.insert(std::pair<unsigned int, std::string>(it->second, it->first));
	}

	for(std::map<std::string, unsigned int>::iterator it = word_all2_huffman.begin(); it != word_all2_huffman.end(); it++ ) {
		lookup_word_all2.insert(std::pair<unsigned int, std::string>(it->second, it->first));
	}
}

HuffmanDecoder::HuffmanDecoder (const char * dirname){
	//Read the maps from disk

	//Note that directory name should exist.
	std::string basedir(dirname);
	std::string target_phrase_path(basedir + "/target_phrases");
	std::string probabilities_path(basedir + "/probs");
	std::string word_all1_path(basedir + "/Wall1");
	std::string word_all2_path(basedir + "/Wall2");

	//Target phrases
	std::ifstream is (target_phrase_path, std::ios::binary);
	boost::archive::text_iarchive iarch(is);
	iarch >> lookup_target_phrase;
	is.close();

	//Probabilities
	std::ifstream is1 (probabilities_path, std::ios::binary);
	boost::archive::text_iarchive iarch1(is1);
	iarch1 >> lookup_probabilities;
	is1.close();

	//Word allignment 1 & 2
	std::ifstream is2 (word_all1_path, std::ios::binary);
	boost::archive::text_iarchive iarch2(is2);
	iarch2 >> lookup_word_all1;
	is2.close();

	std::ifstream is3 (word_all2_path, std::ios::binary);
	boost::archive::text_iarchive iarch3(is3);
	iarch3 >> lookup_word_all2;
	is3.close();
}

HuffmanDecoder::HuffmanDecoder (std::map<unsigned int, std::string> * lookup_target, std::map<unsigned int, std::string> * lookup_prob,
	 std::map<unsigned int, std::string> * lookup_word1, std::map<unsigned int, std::string> * lookup_word2) {
	lookup_target_phrase = *lookup_target;
	lookup_probabilities = *lookup_prob;
	lookup_word_all1 = *lookup_word1;
	lookup_word_all2 = *lookup_word2;
}

target_text HuffmanDecoder::decode_line (std::vector<unsigned int> input){
	//demo decoder
	target_text ret;
	//Split everything
	std::vector<unsigned int> target_phrase;
	std::vector<unsigned int> probs;
	std::vector<unsigned int> wAll;

	//Split the line into the proper arrays
	short num_zeroes = 0;
	int counter = 0;
	while (num_zeroes < 3){
		unsigned int num = input[counter];
		if (num == 0) {
			num_zeroes++;
		} else if (num_zeroes == 0){
			target_phrase.push_back(num);
		} else if (num_zeroes == 1){
			probs.push_back(num);
		} else if (num_zeroes == 2){
			wAll.push_back(num);
		}
		counter++;
	}

	ret.target_phrase = target_phrase;
	ret.word_all1 = splitWordAll1(StringPiece(lookup_word_all1.find(wAll[0])->second));
	ret.word_all2 = splitWordAll2(StringPiece(lookup_word_all2.find(wAll[1])->second));

	//Decode probabilities
	for (std::vector<unsigned int>::iterator it = probs.begin(); it != probs.end(); it++){
		ret.prob.push_back(atof(lookup_probabilities.find(*it)->second.c_str()));
	}

	return ret;

}

inline std::string HuffmanDecoder::getTargetWordFromID(unsigned int id) {
	return lookup_target_phrase.find(id)->second;
}

std::string HuffmanDecoder::getTargetWordsFromIDs(std::vector<unsigned int> ids){
	std::string returnstring;
	for (std::vector<unsigned int>::iterator it = ids.begin(); it != ids.end(); it++){
		returnstring.append(getTargetWordFromID(*it));
	}

	return returnstring;
}