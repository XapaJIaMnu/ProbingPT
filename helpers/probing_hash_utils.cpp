#include "probing_hash_utils.hh"

//Read table from disk, return memory map location
char * readTable(char * filename, size_t size) {
	//Initial position of the file is the end of the file, thus we know the size
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}

	return (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
} 


void serialize_table(char *mem, size_t size, char * filename){
	std::ofstream os (filename, std::ios::binary);	
	os.write((const char*)&mem[0], size);
	os.close();

}