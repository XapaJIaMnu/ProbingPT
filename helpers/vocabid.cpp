#include "vocabid.hh" 

void add_to_map(std::map<uint64_t, std::string> *karta, StringPiece textin){
	//Tokenize
	util::TokenIter<util::SingleCharacter> it(textin, util::SingleCharacter(' '));

	while(it){
		karta->insert(std::pair<uint64_t, std::string>(getHash(*it), it->as_string()));
		it++;
	}
}

void serialize_map(std::map<uint64_t, std::string> *karta, char* filename){
	std::ofstream os (filename, std::ios::binary);
	boost::archive::text_oarchive oarch(os);

	oarch << *karta;  //Serialise map
	os.close();
}

void read_map(std::map<uint64_t, std::string> *karta, char* filename){
	std::ifstream is (filename, std::ios::binary);
	boost::archive::text_iarchive iarch(is);

	iarch >> *karta;

	//Close the stream after we are done.
	is.close();
}

// If necessary those methods could use StringPiece instead of std::string, I don't
// know which is better.
std::string getStringFromID(std::map<uint64_t, std::string> *karta, uint64_t id){
	std::map<uint64_t, std::string>::iterator it;
	it = karta->find(id);
	if (it == karta->end()) {
		return "STRING_NOT_FOUND";
	} else {
		return it->second;
	} 
}

std::string getStringFromIDs(std::map<uint64_t, std::string> *karta, std::vector<uint64_t> ids){
	std::string output = "";
	for (int i = 0; i < ids.size(); i++){
		output = output + getStringFromID(karta, ids[i]) + " ";
	}
	return output;
}

uint64_t getVocabID(std::string candidate){
	std::size_t len = candidate.length();
	uint64_t key = util::MurmurHashNative(candidate.c_str(), len);
	return key;
}

std::vector<uint64_t> getVocabIDs(StringPiece textin){
	//Tokenize
	std::vector<uint64_t> output;

	util::TokenIter<util::SingleCharacter> it(textin, util::SingleCharacter(' '));

	while(it){
		output.push_back(getHash(*it));
		it++;
	}

	return output;
}