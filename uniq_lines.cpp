//The purpose of this program is to get the number of uniq source phrases in the document
//In order to use a smaller probing_hash_table

#include "helpers/line_splitter.hh"

#include "util/file_piece.hh"
#include "util/file.hh"
#include "util/usage.hh"

#include <stdio.h>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "ERROR! The program requires 1 argument, but " << argc - 1 << " were provided." << std::endl;
		std::cout << "Usage: " << argv[0] << " path_to_phrase_table." << std::endl;
	}

	//Read the file
	util::FilePiece filein(argv[1]);

	unsigned long uniq_lines = 0;
	line_text prev_line;

	while (true){
		line_text new_line;
		try {
			//Process line read
			new_line = splitLine(filein.ReadLine());
		} catch (util::EndOfFileException e){
			std::cout << "End of file" << std::endl;
			break;
		}
		if (new_line.source_phrase == prev_line.source_phrase){
			continue;
		} else {
			uniq_lines++;
			prev_line = new_line;
		}
	}

	std::cout << "Number of unique lines is: " << uniq_lines << std::endl;
	return 1;
}
