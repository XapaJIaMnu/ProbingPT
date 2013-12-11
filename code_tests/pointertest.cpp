#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"
#include "util/murmur_hash.hh"
#include "util/probing_hash_table.hh"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>

struct Entry {
  unsigned char key;
  typedef unsigned char Key;

  unsigned char GetKey() const {
    return key;
  }

  void SetKey(unsigned char to) {
    key = to;
  }

  short * GetValue() const {
    return value;
  }

  short * value;
};

//Define table
typedef util::ProbingHashTable<Entry, boost::hash<unsigned char> > Table;

int main() {

  int * arr = new int[4];
  arr[0] = 1;
  arr[1] = 22;
  arr[2] = 3;
  arr[3] = 4;

  int * value;

  value = &arr[0];

  std::cout << "First pointer value: " << *value << std::endl;

  value = value + 1;

  std::cout << "Second pointer value: " << *value << std::endl;

  value = value + 1;

  std::cout << "Third pointer value: " << *value << std::endl;

  return 0;  




  /*
	//Set table size and 
	size_t size = Table::Size(10, 1.2);
  	boost::scoped_array<char> mem(new char[size]);

  	//Why?

  	//Init the table with particular size
  	Table table(mem.get(), size);

  	//Create an entry
	Entry pesho;
	pesho.value = 2333;
	pesho.key = 12;

	//Read entry onto i
	const Entry *i = NULL;

	//Put into table
	table.Insert(pesho);

	table.Find(12, i);

	std::cout << i->GetValue() << std::endl;

	return 1;
  */
}