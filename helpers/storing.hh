#include <stdio.h>
#include <fstream>
#include <iostream>  

#include "helpers/hash.hh"
#include "helpers/probing_hash_utils.hh"
#include "helpers/vocabid.hh"
#include "helpers/huffmanish.hh"
#include <sys/stat.h> //mkdir

#include "util/file_piece.hh"
#include "util/file.hh"

void createProbingPT(const char * phrasetable_path, const char * target_path);

class BinaryFileWriter {
	std::vector<unsigned char> binfile;
	std::vector<unsigned char>::iterator it;
	//Output binary
	std::ofstream os;

public:
	unsigned int dist_from_start = 0; //Distance from the start of the vector.
	uint64_t extra_counter = 0; //After we reset the counter, we still want to keep track of the correct offset, so

	BinaryFileWriter (std::string);
	~BinaryFileWriter ();
	void write (std::vector<unsigned char> * bytes);
	void flush (); //Flush to disk

};