#include "quering.hh"

char * read_binary_file(char * filename){
	//Get filesize
	int fd;
	char * map;
	struct stat filestatus;
	unsigned long filesize = filestatus.st_size;
	stat(filename, &filestatus);

	fd = open(filename, O_RDONLY);

	if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}

	map = (char *)mmap(0, filesize, PROT_READ, MAP_SHARED, fd, 0);

	return map;
} 

QueryEngine::QueryEngine(char * path_to_hashtable, char * path_to_data_bin, char * path_to_vocabid, char * tablesize_ch, char * largest_entry_ch){
	//set largest entry
	largest_entry = atoi(largest_entry_ch);

	//Mmap binary table
	binary_mmaped = read_binary_file(path_to_data_bin);
	
	//Read hashtable
	int tablesize = atoi(tablesize_ch);
	size_t size = Table::Size(tablesize, 1.2);
	char * mem = readTable(path_to_hashtable, size);
	Table table_init(mem, size);
	table = table_init;

	//Read vocabid
	read_map(&vocabids, path_to_vocabid);

	std::cout << "Initialized successfully! " << std::endl;
}

QueryEngine::~QueryEngine(){
	std::cout << "TODO! IMPLEMENT DESTRUCTOR!" << std::endl;
	/*stub
	munmap(mem, size);
	munmap(map, filesize);
	*/
}

std::pair<bool, std::vector<target_text>> QueryEngine::query(std::vector<uint64_t> source_phrase){
	bool found;
	std::vector<target_text> translation_entries;
	const Entry * entry;
	//TOO SLOW
	//uint64_t key = util::MurmurHashNative(&source_phrase[0], source_phrase.size());
	uint64_t key = 0;
	for (int i = 0; i < source_phrase.size(); i++){
		key += source_phrase[i];
	}


	found = table.Find(key, entry);

	if (found){
		//The phrase that was searched for was found! We need to get the translation entries.
		//We will read the largest entry in bytes and then filter the unnecesarry with functions
		//from line_splitter
		std::string text_entry(&binary_mmaped[entry -> GetValue()] , &binary_mmaped[entry -> GetValue()] + largest_entry);
		StringPiece raw_string = StringPiece(text_entry);

		//Get only the translation entries necessary
		translation_entries = splitTargetLine(raw_string);

	}

	std::pair<bool, std::vector<target_text>> output (found, translation_entries);

	return output;
	
}

std::pair<bool, std::vector<target_text>> QueryEngine::query(StringPiece source_phrase){
	bool found;
	std::vector<target_text> translation_entries;
	const Entry * entry;
	//Convert source frase to VID
	std::vector<uint64_t> source_phrase_vid = getVocabIDs(source_phrase);
	//TOO SLOW
	//uint64_t key = util::MurmurHashNative(&source_phrase_vid[0], source_phrase_vid.size());
	uint64_t key = 0;
	for (int i = 0; i < source_phrase_vid.size(); i++){
		key += source_phrase_vid[i];
	}

	found = table.Find(key, entry);


	if (found){
		//The phrase that was searched for was found! We need to get the translation entries.
		//We will read the largest entry in bytes and then filter the unnecesarry with functions
		//from line_splitter
		std::string text_entry(&binary_mmaped[entry -> GetValue()] , &binary_mmaped[entry -> GetValue()] + largest_entry);
		StringPiece raw_string = StringPiece(text_entry);

		//Get only the translation entries necessary
		translation_entries = splitTargetLine(raw_string);

	}

	std::pair<bool, std::vector<target_text>> output (found, translation_entries);

	return output;

}