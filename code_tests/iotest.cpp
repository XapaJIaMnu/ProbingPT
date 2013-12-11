#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"

#include <fstream>
#include <iostream>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	std::fstream backing("testfile", std::ios::in);
	util::FilePiece test(backing);
	//An example of how not to read a file
	while(true){
		try {
			std::string num = (test.ReadLine()).as_string();
			int number = atoi(num.c_str());
			std::cout << num << std::endl;
		} catch (util::EndOfFileException e){
			std::cout << "End of file" << std::endl;
			break;
		}
	}
	return 0;
}