#include <map> //Map for vocab ids

#include "helpers/hash.hh"
#include "helpers/vocabid.hh"

int main(int argc, char* argv[]){

	//Create a map and serialize it
	std::map<uint64_t, std::string> vocabids;
	StringPiece demotext = StringPiece("Demo text with 3 elements");
	add_to_map(&vocabids, demotext);
	//Serialize map
	serialize_map(&vocabids, "/tmp/testmap.bin");

	//Read the map and test if the values are the same
	std::map<uint64_t, std::string> newmap;
	read_map(&newmap, "/tmp/testmap.bin");

	//Used hashes
	uint64_t num1 = getHash(StringPiece("Demo"));
	uint64_t num2 = getHash(StringPiece("text"));
	uint64_t num3 = getHash(StringPiece("with"));
	uint64_t num4 = getHash(StringPiece("3"));
	uint64_t num5 = getHash(StringPiece("elements"));

	//Tests
	bool test1 = newmap.find(num1)->second == vocabids.find(num1)->second;
	bool test2 = newmap.find(num2)->second == vocabids.find(num2)->second;
	bool test3 = newmap.find(num3)->second == vocabids.find(num3)->second;
	bool test4 = newmap.find(num4)->second == vocabids.find(num4)->second;
	bool test5 = newmap.find(num5)->second == vocabids.find(num5)->second;

	if (test1 && test2 && test3 && test4 && test5){
		std::cout << "Map was successfully written and read!" << std::endl;
	} else {
		std::cout << "Error! " << test1 << " " << test2 << " " << test3 << " " << test4 << " " << test5 << " " << std::endl;
	}
	

	return 1;
	
} 
