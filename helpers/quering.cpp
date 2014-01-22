#include "quering.hh"

char * read_binary_file(char * filename, size_t filesize){
	//Get filesize
	int fd;
	char * map;

	fd = open(filename, O_RDONLY);

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

	return map;
} 

QueryEngine::QueryEngine(char * path_to_hashtable, char * path_to_data_bin, char * path_to_vocabid, char * tablesize_ch, char * largest_entry_ch){
	//set largest entry
	largest_entry = atoi(largest_entry_ch);

	//Mmap binary table
	struct stat filestatus;
	stat(path_to_data_bin, &filestatus);
	binary_filesize = filestatus.st_size;
	binary_mmaped = read_binary_file(path_to_data_bin, binary_filesize);

	//Read hashtable
	int tablesize = atoi(tablesize_ch);
	size_t table_filesize = Table::Size(tablesize, 1.2);
	mem = readTable(path_to_hashtable, table_filesize);
	Table table_init(mem, table_filesize);
	table = table_init;

	//Read vocabid
	read_map(&vocabids, path_to_vocabid);

	std::cout << "Initialized successfully! " << std::endl;
}

QueryEngine::~QueryEngine(){
	//Clear mmap content from memory.
	munmap(binary_mmaped, binary_filesize);
	munmap(mem, table_filesize);
	
}

std::pair<bool, std::vector<target_text> > QueryEngine::query(std::vector<uint64_t> source_phrase){
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
		uint64_t initial_index = entry -> GetValue();
		uint64_t end_index = initial_index + largest_entry;
		//At the end of the file we can't readd + largest_entry cause we get a segfault.
		//Instead read till the end of the file.
		if (end_index > binary_filesize){
			end_index = binary_filesize;
		}
		std::string text_entry(&binary_mmaped[initial_index] , &binary_mmaped[end_index]);
		StringPiece raw_string = StringPiece(text_entry);

		//Get only the translation entries necessary
		translation_entries = splitTargetLine(raw_string);

	}

	std::pair<bool, std::vector<target_text> > output (found, translation_entries);

	return output;
	
}

std::pair<bool, std::vector<target_text> > QueryEngine::query(StringPiece source_phrase){
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

		uint64_t initial_index = entry -> GetValue();
		uint64_t end_index = initial_index + largest_entry;
		//At the end of the file we can't readd + largest_entry cause we get a segfault.
		//Instead read till the end of the file.
		if (end_index > binary_filesize){
			end_index = binary_filesize;
		}
		std::string text_entry(&binary_mmaped[initial_index] , &binary_mmaped[end_index]);
		StringPiece raw_string = StringPiece(text_entry);

		//Get only the translation entries necessary
		translation_entries = splitTargetLine(raw_string);

	}

	std::pair<bool, std::vector<target_text> > output (found, translation_entries);

	return output;

}

void QueryEngine::printTargetInfo(std::vector<target_text> target_phrases){
	int entries = target_phrases.size();

	for (int i = 0; i<entries; i++){
		std::cout << "Entry " << i+1 << " of " << entries << ":" << std::endl;
		//Print text
		std::cout << getStringFromIDs(&vocabids, target_phrases[i].target_phrase) << "\t";
		
		//Print probabilities:
		for (int j = 0; j<target_phrases[i].prob.size(); j++){
			std::cout << target_phrases[i].prob[j] << " ";
		}
		std::cout << "\t";

		//Print word_all1
		for (int j = 0; j<target_phrases[i].word_all1.size(); j++){
			if (j%2 == 0){
				std::cout << target_phrases[i].word_all1[j] << "-";
			}else{
				std::cout << target_phrases[i].word_all1[j] << " ";
			}
		}
		std::cout << "\t";

		//print word_all2
		for (int j = 0; j<target_phrases[i].word_all2.size(); j++){
			std::cout << target_phrases[i].word_all1[j] << " ";
		}
		std::cout << std::endl;
	}
}