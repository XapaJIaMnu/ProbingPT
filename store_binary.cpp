#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"
#include "util/tokenize_piece.hh"
#include "util/murmur_hash.hh"
#include "util/probing_hash_table.hh"
#include "util/usage.hh"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>

//Hash table entry
struct Entry {
	uint64_t key;
	typedef unsigned char Key;

	uint64_t GetKey() const {
		return key;
	}

	void SetKey(uint64_t to) {
		key = to;
	}

	uint64_t GetValue() const {
		return value;
	}

	uint64_t value;
};

//Define table
typedef util::ProbingHashTable<Entry, boost::hash<uint64_t> > Table;

//Struct for holding processed line
struct line_text {
	StringPiece source_phrase;
	StringPiece target_phrase;
	StringPiece prob;
	StringPiece word_all1;
	StringPiece word_all2;
};

//Ask if it's better to have it receive a pointer to a line_text struct
line_text splitLine(StringPiece textin);

bool test_tokenization();
bool test_vectorinsert();
void test();

//Appends to the vector used for outputting.
int vector_append(line_text *input, std::vector<char> *outputvec);

//Gets the MurmurmurHash for give string
uint64_t getHash(StringPiece text);
void serialize_table(char *mem, size_t size, char * filename);

uint64_t getHash(StringPiece text) {
	std::size_t len = text.size();
	uint64_t key = util::MurmurHashNative(text.data(), len);
	return key;
}

void serialize_table(char *mem, size_t size, char * filename){
	std::ofstream os (filename, std::ios::binary);	
	os.write((const char*)&mem[0], size);
	os.close();

}

std::vector<char>::iterator vector_append(line_text* input, std::vector<char>* outputvec, std::vector<char>::iterator it){
	//Append everything to one string
	std::string temp = "";
	int vec_size = 0;
	int new_string_size = 0;

	temp += input->target_phrase.as_string() + " " + input->prob.as_string() + " " + input->word_all1.as_string() + " " + input->word_all2.as_string();
	
	//Put into vector
	outputvec->insert(it, temp.begin(), temp.end());

	//Return new iterator updated iterator
	return it+temp.length();
}

line_text splitLine(StringPiece textin) {
	const char delim[] = " ||| ";
	std::string temp; //Temp string for conversion
	int num;
	line_text output;

	//Tokenize
	util::TokenIter<util::MultiCharacter> it(textin, util::MultiCharacter(delim));
	//Get source phrase
	output.source_phrase = *it;
	it++;
	//Get target_phrase
	output.target_phrase = *it;
	it++;
	//Get probabilities
	output.prob = *it;
	it++;
	//Get WordAllignment 1
	output.word_all1 = *it;
	it++;
	//Get WordAllignment 2
	output.word_all2 = *it;

	return output;
}

int main(int argc, char* argv[]){
	if (argc != 5) {
		// Tell the user how to run the program
		std::cerr << "Provided " << argc << " arguments, needed 4." << std::endl;
		std::cerr << "Usage: " << argv[0] << " path_to_phrasetable number_of_lines output_bin_file output_hash_table" << std::endl;
		return 1;
	}

	//Read the file
	util::FilePiece filein(argv[1]);
	int tablesize = atoi(argv[2]);

	//Init the table
	size_t size = Table::Size(tablesize, 1.2);
	char * mem = new char[size];
	memset(mem, 0, size);
	Table table(mem, size);

	//Output binary
	std::ofstream os (argv[3], std::ios::binary);
	
	//Vector with 10000 elements, after the 9000nd we swap to disk, we have 10000 for buffer
	std::vector<char> ram_container;
	ram_container.reserve(10000);
	std::vector<char>::iterator it = ram_container.begin();
	unsigned int dist_from_start = 0;
	uint64_t extra_counter = 0; //After we reset the counter, we still want to keep track of the correct offset, so
									//we should keep an extra counter for that reason.


	//Read everything and processs
	while(true){
		//Calculate offset
		dist_from_start = distance(ram_container.begin(),it);
		try {
			//Process line read
			line_text line;
			line = splitLine(filein.ReadLine());

			//Create an entry to keep track of the offset
			Entry pesho;
			pesho.value = dist_from_start + extra_counter;
			pesho.key = getHash(line.source_phrase);

			//Put into table
			table.Insert(pesho);

			it = vector_append(&line, &ram_container, it); //Put into the array and update iterator to the new end position

			//Write to disk if over 10000
			if (pesho.value > 9000) {
				std::cout << "Dist counter " << dist_from_start << std::endl;
				std::cout << "Extra counter " << extra_counter << std::endl;
				//write to memory
				os.write(&ram_container[0], dist_from_start);
				//Clear the vector:
				ram_container.clear();
				ram_container.reserve(10000);
				extra_counter += dist_from_start;
				it = ram_container.begin(); //Reset iterator
			}
		} catch (util::EndOfFileException e){
			std::cout << "End of file" << std::endl;
			os.write(&ram_container[0], dist_from_start);
			break;
		}
	}

	serialize_table(mem, size, argv[4]);
	os.close();
	util::PrintUsage(std::cout);

	const Entry * tmp;

	StringPiece test = StringPiece("! ! ! !");
	uint64_t temp_hash = getHash(test);
	bool found = table.Find(temp_hash, tmp);
	std::cout << "Hash of test is " << temp_hash << std::endl;
	std::cout << "Found is " << found << std::endl;

	std::cout << "Position is " << tmp -> GetValue() << std::endl;

	std::cout << "Extra counter is " << extra_counter << std::endl;

	return 1;
}

bool test_tokenization(){
	StringPiece line1 = StringPiece("! ! ! ! ||| ! ! ! ! ||| 0.0804289 0.141656 0.0804289 0.443409 2.718 ||| 0-0 1-1 2-2 3-3 ||| 1 1 1");
	StringPiece line2 = StringPiece("! ! ! ) , has ||| ! ! ! ) - , a ||| 0.0804289 0.0257627 0.0804289 0.00146736 2.718 ||| 0-0 1-1 2-2 3-3 4-4 4-5 5-6 ||| 1 1 1");
	StringPiece line3 = StringPiece("! ! ! ) , ||| ! ! ! ) - , ||| 0.0804289 0.075225 0.0804289 0.00310345 2.718 ||| 0-0 1-1 2-2 3-3 4-4 4-5 ||| 1 1 1");
	StringPiece line4 = StringPiece("! ! ! ) ||| ! ! ! ) . ||| 0.0804289 0.177547 0.0268096 0.000872597 2.718 ||| 0-0 1-1 2-2 3-3 ||| 1 3 1");

	line_text output1 = splitLine(line1);
	line_text output2 = splitLine(line2);
	line_text output3 = splitLine(line3);
	line_text output4 = splitLine(line4);

	bool test1 = output1.prob == StringPiece("0.0804289 0.141656 0.0804289 0.443409 2.718");
	bool test2 = output2.word_all1 == StringPiece("0-0 1-1 2-2 3-3 4-4 4-5 5-6");
	bool test3 = output2.target_phrase == StringPiece("! ! ! ) - , a");
	bool test4 = output3.source_phrase == StringPiece("! ! ! ) ,");
	bool test5 = output4.word_all2 == StringPiece("1 3 1");

	//std::cout << test1 << " " << test2 << " " << test3 << " " << test4 << std::endl;

	if (test1 && test2 && test3 && test4 && test5){
		return true;
	}else{
		return false;
	}

}

bool test_vectorinsert() {
	StringPiece line1 = StringPiece("! ! ! ! ||| ! ! ! ! ||| 0.0804289 0.141656 0.0804289 0.443409 2.718 ||| 0-0 1-1 2-2 3-3 ||| 1 1 1");
	StringPiece line2 = StringPiece("! ! ! ) , has ||| ! ! ! ) - , a ||| 0.0804289 0.0257627 0.0804289 0.00146736 2.718 ||| 0-0 1-1 2-2 3-3 4-4 4-5 5-6 ||| 1 1 1");
	line_text output = splitLine(line1);
	line_text output2 = splitLine(line2);

	//Init container vector and iterator.
	std::vector<char> container;
	container.reserve(10000); //Reserve vector
	std::vector<char>::iterator it = container.begin();

	//Put a value into the vector
	it = vector_append(&output, &container, it);
	it = vector_append(&output2, &container, it);

	std::string test(container.begin(), container.end());
	std::string should_be = "! ! ! ! 0.0804289 0.141656 0.0804289 0.443409 2.718 0-0 1-1 2-2 3-3 1 1 1! ! ! ) - , a 0.0804289 0.0257627 0.0804289 0.00146736 2.718 0-0 1-1 2-2 3-3 4-4 4-5 5-6 1 1 1";
	if (test == should_be) {
		return true;
	} else {
		return false;
	}
}

void test(){
	bool test_res = test_tokenization();
	bool test_res2 = test_vectorinsert();
	if (test_res){
		std::cout << "Test passes" << std::endl;
	} else {
		std::cout << "Tokenization test fails" << std::endl;
	}

	if (test_res2){
		std::cout << "Test passes" << std::endl;
	} else {
		std::cout << "Vector insert test fails" << std::endl;
	}
}
