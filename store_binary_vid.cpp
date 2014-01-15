#include "util/file_piece.hh"
#include "util/file.hh"
#include "util/usage.hh"

#include <stdio.h>
#include <fstream>
#include <iostream>

#include <ctime> //for timing.
#include <chrono>

#include "helpers/hash.hh"
#include "helpers/line_splitter.hh"
#include "helpers/probing_hash_utils.hh"
#include "helpers/vocabid.hh"


//Appends to the vector used for outputting.
int vector_append(line_text *input, std::vector<char> *outputvec, bool new_entry);

std::vector<char>::iterator vector_append(line_text* input, std::vector<char>* outputvec, std::vector<char>::iterator it, bool new_entry){
	//Append everything to one string
	std::string temp = "";
	int vec_size = 0;
	int new_string_size = 0;

	if (new_entry){
		//If we have a new entry, add an empty line.
		temp += '\n';
	}

	temp += input->target_phrase.as_string() + "\t" + input->prob.as_string() + "\t" + input->word_all1.as_string() + "\t" + input->word_all2.as_string() + "\n";
	
	//Put into vector
	outputvec->insert(it, temp.begin(), temp.end());

	//Return new iterator updated iterator
	return it+temp.length();
}

int main(int argc, char* argv[]){

	//Time everything
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();


	if (argc != 6) {
		// Tell the user how to run the program
		std::cerr << "Provided " << argc << " arguments, needed 4." << std::endl;
		std::cerr << "Usage: " << argv[0] << " path_to_phrasetable number_of_uniq_lines output_bin_file output_hash_table output_vocab_id" << std::endl;
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

	//Vocab id
	std::map<uint64_t, std::string> vocabids;

	//Output binary
	std::ofstream os (argv[3], std::ios::binary);
	
	//Vector with 10000 elements, after the 9000nd we swap to disk, we have 10000 for buffer
	std::vector<char> ram_container;
	ram_container.reserve(10000);
	std::vector<char>::iterator it = ram_container.begin();
	unsigned int dist_from_start = 0;
	uint64_t extra_counter = 0; //After we reset the counter, we still want to keep track of the correct offset, so
									//we should keep an extra counter for that reason.
	line_text prev_line; //Check if the source phrase of the previous line is the same
	int longestchars = 0; //Keep track of what is the maximum number of characters we need to read when quering


	//Read everything and processs
	while(true){
		//Calculate offset
		dist_from_start = distance(ram_container.begin(),it);
		try {
			//Process line read
			line_text line;
			line = splitLine(filein.ReadLine());

			if (line.source_phrase != prev_line.source_phrase){
				//Add to vocabid
				add_to_map(&vocabids, line.source_phrase);
				add_to_map(&vocabids, line.target_phrase);
				//Create an entry only for different source phrases.
				//Create an entry to keep track of the offset
				Entry pesho;
				pesho.value = dist_from_start + extra_counter;
				pesho.key = getHash(line.source_phrase);

				//Put into table
				table.Insert(pesho);
				prev_line = line;
				it = vector_append(&line, &ram_container, it, true); //Put into the array and update iterator to the new end position
			} else{
				add_to_map(&vocabids, line.target_phrase);
				it = vector_append(&line, &ram_container, it, false); //Put into the array and update iterator to the new end position
			}

			//Write to disk if over 10000
			if (dist_from_start > 9000) {
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
	
	//clean up
	os.close();
	ram_container.clear();
	delete[] mem;

	//Serialize vocabids
	serialize_map(&vocabids, argv[5]);
	vocabids.clear();

	//End timing
	std::clock_t c_end = std::clock();
	auto t_end = std::chrono::high_resolution_clock::now();

	//Print timing results
	std::cout << "CPU time used: "<< 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC<< " ms\n";
	std::cout << "Real time passed: "<< std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count()<< " ms\n";


	util::PrintUsage(std::cout);
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
	it = vector_append(&output, &container, it, false);
	it = vector_append(&output2, &container, it, false);

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
