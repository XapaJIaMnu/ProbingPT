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
#include <string>

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

void serialize_table(boost::scoped_array<char> *mem, size_t size, char * filename);

void readTable(char * filename, boost::scoped_array<char> *mem, size_t size);

void readTable(char * filename, boost::scoped_array<char> *mem, size_t size) {
	//Initial position of the file is the end of the file, thus we know the size
	std::ifstream file (filename, std::ios::in|std::ios::binary);
	file.read ((char *)mem->get(), size); // read
	file.close();
}


void serialize_table(boost::scoped_array<char> *mem, size_t size, char * filename){
	std::ofstream os (filename, std::ios::binary);	
	os.write((const char*)&size, sizeof(size));
	os.write((const char*)&mem[0], size);
	os.close();

}

int main() {

	StringPiece tohash1 = StringPiece("Hash text");
 	int num1 = 1234;
 	int num2 = 3211;
 	StringPiece tohash2 = StringPiece("I REALLY. Like pies.");

	std::size_t len1 = tohash1.size();
	uint64_t key1 = util::MurmurHashNative(&tohash1, len1);
	std::size_t len2 = tohash1.size();
	uint64_t key2 = util::MurmurHashNative(&tohash2, len2);

	//Make entries.
	Entry entry1, entry2;
	entry1.key = key1;
	entry1.value = num1;
	entry2.key = key2;
	entry2.value = num2;

	//Init the table
	size_t size = Table::Size(3, 1.2);
	boost::scoped_array<char> mem(new char[size]);
	Table table(mem.get(), size);

	table.Insert(entry1);
	table.Insert(entry2);

	const Entry * find_num;
	table.Find(key1, find_num);
	std::cout << "Num 1 is " << find_num -> GetValue() << std::endl;

	//Serialize to disk test
	serialize_table(&mem, size, "hashtable.dat");

	boost::scoped_array<char> read(new char[size]);

	readTable("hashtable.dat", &read, size);

	serialize_table(&read, size, "readtable2.dat");

	Table table2(read.get(), size);
	std::cout << "Table assigned! " << std::endl;
	table2.CheckConsistency();
	std::cout << "Table is consistent " << std::endl;

	for (int i = 0; i < size; i++){
		if (mem[i] == read[i]){
		}else{
			std::cout << "Error at " << i << std::endl;
			break;
		}
	}

	const Entry* find_num2;
	table2.Find(key2, find_num2);
	std::cout << "Entry found! " << std::endl;
	std::cout << "Num 1 is " << find_num2 -> GetValue() << std::endl;


	return 0;
}