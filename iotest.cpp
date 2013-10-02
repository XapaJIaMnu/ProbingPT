#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"

#include <fstream>
#include <iostream>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	std::fstream backing("testfile", std::ios::in);
	util::FilePiece test(backing);
	while(true){
		try {
			std::cout << test.ReadLine();
		} catch (int e){
			return 1;
		}
	}
}