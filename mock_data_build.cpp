#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"
#include "util/tokenize_piece.hh"
#include "util/murmur_hash.hh"
#include "util/probing_hash_table.hh"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>

//Struct for holding processed line
struct line_text {
	int value;
	StringPiece text;
};

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

	unsigned int GetValue() const {
		return value;
	}

	unsigned int value;
};

//Define table
typedef util::ProbingHashTable<Entry, boost::hash<uint64_t> > Table;

void serialize_table(char *mem, size_t size, char * filename);

void readTable(char * filename, char *mem, size_t size);

//Ask if it's better to have it receive a pointer to a line_text struct
line_text splitLine(StringPiece textin);

//Gets the MurmurmurHash for give string
uint64_t getHash(StringPiece text);

uint64_t getHash(StringPiece text) {
	std::size_t len = text.size();
	uint64_t key = util::MurmurHashNative(&text, len);
	return key;
}

void readTable(char * filename, char *mem, size_t size) {
	//Initial position of the file is the end of the file, thus we know the size
	std::ifstream file (filename, std::ios::in|std::ios::binary);
	file.read ((char *)mem, size); // read
	file.close();
}


void serialize_table(char *mem, size_t size, char * filename){
	std::ofstream os (filename, std::ios::binary);	
	os.write((const char*)&mem[0], size);
	os.close();

}

line_text splitLine(StringPiece textin) {
	const char delim[] = " ||| ";
	StringPiece str; //Temp string container
	std::string temp; //Temp string for conversion
	int num;
	line_text output;

	//Tokenize
	util::TokenIter<util::MultiCharacter> it(textin, util::MultiCharacter(delim));
	//Get string
	str = *it;
	output.text = str;
	it++;

	//Get num
	str = *it;

	//Convert to int
	temp = str.as_string();
	num = atoi(temp.c_str());
	output.value = num;

	return output;
}

int main(int argc, char* argv[]){

	//Input file and table size
	/*if (argc != 2) {
		std::cout << "Provide input file and number of lines!" << std::endl;
		return 1;
	}*/
	std::fstream fileread("source.uniq.rand", std::ios::in);
	util::FilePiece filein(fileread);
	int tablesize = 23339836;

	//Init the table
	size_t size = Table::Size(tablesize, 1.2);
	char * mem = new char[size];
	memset(mem, 0, size);
	Table table(mem, size);

	//Output binary
	std::ofstream os ("data.dat", std::ios::binary);
	
	//Vector with 1000 elements, after the 1000nd we swap to disk
	std::vector<int> ram_container(1000);
	short counter = 0; //Counts how many entires are in the vector
	unsigned int offset = 0; //Keeps track of the offset

	//debuging variables
	Entry kiro;
	line_text temp;

	while(true){
		try {
			//Process line read
			line_text line;
			line = splitLine(filein.ReadLine());

			ram_container[counter] = line.value; //Put into the array

			//Create an entry to keep track of the offset
			Entry pesho;
			pesho.value = offset;
			pesho.key = getHash(line.text);

			//Put into table
			table.Insert(pesho);
			counter++;
			offset++;

			//debugging
			kiro = pesho;
			temp.value = line.value;
			temp.text = line.text;

			//Write to memory
			if (counter == 1000) {
				//write to memory
				os.write((const char*)&ram_container[0], counter * sizeof(int));
				counter = 0;
			}
		} catch (util::EndOfFileException e){
			std::cout << "End of file" << std::endl;
			os.write((const char*)&ram_container[0], counter * sizeof(int));
			break;
		}
	}
	//Close write stream:
	os.close();
	std::cout << "Last offset was " << offset << std::endl;
	std::cout << "Last entry key " << kiro.key << " Value " << kiro.value << std::endl;
	std::cout << "Last line text " << temp.text << " Num " << temp.value << std::endl;
	std::cout << "Expected hash is " << getHash(temp.text) << std::endl;

	const Entry * tocheck;
	table.Find(kiro.key, tocheck);
	std::cout << "Tocheck key " << tocheck -> GetKey() << " Tocheck value " << tocheck -> GetValue() << std::endl;

	serialize_table(mem, size, "hashtable.dat");
	return 0;

}