#include "util/file_piece.hh"

#include "util/file.hh"
#include "util/scoped.hh"
#include "util/string_piece.hh"
#include "util/murmur_hash.hh"
#include "util/probing_hash_table.hh"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

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

  int * GetValue() const {
    return value;
  }

  int * value;
};

//Define table
typedef util::ProbingHashTable<Entry, boost::hash<unsigned char> > Table;

int main() {
  /*This is a simple proof of concept that has the additional purpose to make me more comfortable with the data structures used.
    The idea is that we are going to take a file and store its information efficiently in a compressed array and use the probing
    hash table to maintain indices to the elements*/

  //Set table size to the number of entries.
  size_t size = Table::Size(9, 1.2);
  boost::scoped_array<char> mem(new char[size]);

  //Why?

  //Init the table with particular size
  Table table(mem.get(), size);
    

  std::vector<int> compressed_container;
  unsigned char counter = 0; //Hacky, don't ask.

  std::fstream backing("testfile", std::ios::in);
  util::FilePiece test(backing);
  //An example of how not to read a file (try/catch), will change later, but for now, hacking
  while(true){
    try {
      std::string num = (test.ReadLine()).as_string();
      int number = atoi(num.c_str());
      compressed_container.push_back(number); //Put into the array

      //Create an entry
      Entry pesho;
      pesho.value = &compressed_container[counter];
      std::cout << "Pesho test " << *(pesho.GetValue()) << std::endl;
      pesho.key = counter;

      //Put into table
      table.Insert(pesho);
      counter++;
    } catch (util::EndOfFileException e){
      std::cout << "End of file" << std::endl;
      break;
    }
  }

  //NOBODY should be using unsigned chars as a counter, so please ignore that hack for now (:)
  for (unsigned char i = 0; i < counter; i++){
    const Entry *temp = NULL;
    table.Find(i, temp);
    int * res = temp->GetValue();
    std::cout << "Value at entry " << (unsigned short) (i) << " is " << *res << std::endl;
  }


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