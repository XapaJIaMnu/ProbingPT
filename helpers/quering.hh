#include "line_splitter.hh"
#include "probing_hash_utils.hh"
#include "vocabid.hh"
#include <sys/stat.h> //For finding size of file
#include <sys/mman.h>

char * read_binary_file(char * filename);

class QueryEngine {
	char * binary_mmaped;
	std::map<uint64_t, std::string> vocabids;

	Table table;
	char *mem; //Memory for the table, necessary so that we can correctly destroy the object

	int largest_entry; //Size of largest entry, for reading from binary_mmaped when quering

	size_t binary_filesize;
	size_t table_filesize;
	public:
		QueryEngine (const char *, const char *, const char *, const char *, const char *);
		~QueryEngine();
		std::pair<bool, std::vector<target_text> > query(StringPiece source_phrase);
		std::pair<bool, std::vector<target_text> > query(std::vector<uint64_t> source_phrase);
		void printTargetInfo(std::vector<target_text> target_phrases);

};


