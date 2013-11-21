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
#include <sstream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h> //For finding size of file
#include <boost/functional/hash.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char * readTable(char * filename, size_t size);

uint64_t getHash(StringPiece text);

uint64_t getHash(StringPiece text) {
	std::size_t len = text.size();
	uint64_t key = util::MurmurHashNative(text.data(), len);
	return key;
}

/*
void readTable(char * filename, char *mem, size_t size) {
	std::cerr << "Normal IO" << std::endl;
	//Initial position of the file is the end of the file, thus we know the size
	std::ifstream file (filename, std::ios::in|std::ios::binary);
	file.read ((char *)mem, size); // read
	for (int i = 0; i < 10; i++){
		std::cout << "First " << mem[i] << "Last " << mem[size-i] << std::endl;
	}
	file.close();
}
*/

char * readTable(char * filename, size_t size) {
    std::cerr << "Mem map" << std::endl;
	//Initial position of the file is the end of the file, thus we know the size
	int fd;
	//char *map;  // mmapped char array

	//Find the size
	struct stat filestatus;
	stat(filename, &filestatus);
	unsigned long filesize = filestatus.st_size;
	int array_length = filesize/sizeof(char);
	std::cout << "Table length detected is " << array_length << std::endl;
	std::cout << "Table length calculated is " << size << std::endl;


	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}

	return (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
}

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

int main(int argc, char* argv[]) {
	if (argc != 4) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " path_to_hashtable path_to_data_bin tablesize" << std::endl;
		return 1;
	}

	int i;
	int fd;
	char *map;  /* mmapped array of int's */

	//Find the size
	struct stat filestatus;
	stat(argv[2], &filestatus);
	unsigned long filesize = filestatus.st_size;
	int array_length = filesize/sizeof(char) - 1; //The end of file has \0, which we don't want to count.
	std::cout << "array_length is " << array_length << std::endl;

	fd = open(argv[2], O_RDONLY);
	if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}

	map = (char *)mmap(0, filesize, PROT_READ, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
	}

	//End of memory mapping code

	//Init the table
	int tablesize = atoi(argv[3]);
	size_t size = Table::Size(tablesize, 1.2);

	char * mem = readTable(argv[1], size);
	Table table(mem, size);
	for (int i = 0; i < 10; i++){
		std::cout << "First " << mem[i] << "Last " << mem[size-i] << std::endl;
	}
	table.CheckConsistency();
	std::cout << "Table is consistent!" << std::endl;

	//Get a key;
	StringPiece test = StringPiece("! ! ! !");
	uint64_t key = getHash(test);

	std::cout << "Hash is " << key << std::endl;

	const Entry * tmp;
	bool keyfound = table.Find(key, tmp);
	std::cout <<"Key found" << keyfound << std::endl;
	//unsigned int idx = tmp -> GetValue();
	//std::cout << "Index is " << idx << std::endl;
	std::string test_str(&map[15], &map[115]);
	std::cout << "String is " << test_str << std::endl;

	//Interactive search
	std::cout << "Please enter a string to be searched, or exit to exit." << std::endl;
	while (true){
		bool found;
		std::string cinstr = "";
		getline(std::cin, cinstr);
		if (cinstr == "exit"){
			break;
		}else{
			StringPiece tofind = StringPiece(cinstr);
			key = getHash(cinstr);
			found = table.Find(key, tmp);
			if (found) {
				std::string found(&map[tmp -> GetValue()] , &map[tmp -> GetValue()] + 100);
				std::cout << "Integer corresponding to " << cinstr << " is " << found << std::endl;
			} else {
				std::cout << "Key not found!" << std::endl;
			}
			
		}
	}
	//clean up
	delete[] mem;
	close(fd);
	util::PrintUsage(std::cout);

	return 0;
}