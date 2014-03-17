#include "storing.hh"

BinaryFileWriter::BinaryFileWriter (std::string basepath) : os ((basepath + "/binfile.dat").c_str(), std::ios::binary) {
	binfile.reserve(10000); //Reserve part of the vector to avoid realocation
	it = binfile.begin();
}

void BinaryFileWriter::write (std::vector<unsigned char> * bytes) {
	binfile.insert(it, bytes->begin(), bytes->end()); //Insert the bytes
	//Keep track of the offsets
	it += bytes->size();
	dist_from_start = distance(binfile.begin(),it);
	//Flush the vector to disk every once in a while so that we don't consume too much ram
	if (dist_from_start > 9000) {
		flush();
	}
}

void BinaryFileWriter::flush () {
	//Cast unsigned char to char before writing...
	os.write((char *)&binfile[0], dist_from_start);
	//Clear the vector:
	binfile.clear();
	binfile.reserve(10000);
	extra_counter += dist_from_start; //Keep track of the total number of bytes.
	it = binfile.begin(); //Reset iterator
	dist_from_start = distance(binfile.begin(),it); //Reset dist from start
}

BinaryFileWriter::~BinaryFileWriter (){
	os.close();
	binfile.clear();
}

void createProbingPT(const char * phrasetable_path, const char * target_path){
	//Get basepath and create directory if missing
	std::string basepath(target_path);
	mkdir(basepath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Set up huffman and serialize decoder maps.
	Huffman huffmanEncoder(phrasetable_path); //initialize
	huffmanEncoder.assign_values();
	huffmanEncoder.produce_lookups();
	huffmanEncoder.serialize_maps(target_path);

	//Get uniq lines:
	unsigned long uniq_entries = huffmanEncoder.getUniqLines();

	//Read the file
	util::FilePiece filein(phrasetable_path);

	//Init the probing hash table
	size_t size = Table::Size(uniq_entries, 1.2);
	char * mem = new char[size];
	memset(mem, 0, size);
	Table table(mem, size);

	BinaryFileWriter binfile(basepath); //Init the binary file writer.

	line_text prev_line; //Check if the source phrase of the previous line is the same
	int longestchars = 0; //Keep track of what is the maximum number of characters we need to read when quering
	int currentlong = 0; //How long is the current

	//Keep track of the size of each group of target phrases
	uint64_t entrystartidx;
	unsigned long entryendidx;
	//uint64_t line_num = 0;


	//Read everything and processs
	while(true){
		//line_num++;
		try {
			//Process line read
			line_text line;
			line = splitLine(filein.ReadLine());
			//std::cout << "Reached line number: " << line_num << std::endl;

			if (line.source_phrase != prev_line.source_phrase){

				if ((binfile.dist_from_start + binfile.extra_counter) != 0) {
					//If this is the first line of the phrase table we shouldn't
					//Create a new entry even

					//Create an entry for the previous source phrase:
					Entry pesho;
					pesho.value = entrystartidx;
					//The key is the sum of hashes of individual words. Probably not entirerly correct, but fast
					pesho.key = 0;
					std::vector<uint64_t> vocabid_source = getVocabIDs(line.source_phrase);
					for (int i = 0; i < vocabid_source.size(); i++){
						pesho.key += vocabid_source[i];
					}
					pesho.bytes_toread = binfile.dist_from_start + binfile.extra_counter - entrystartidx;

					//FInd out how long the longest entry is:
					if (pesho.bytes_toread > longestchars) {
						longestchars = pesho.bytes_toread;
					}
					//Put into table
					table.Insert(pesho);

				}
				entrystartidx = binfile.dist_from_start + binfile.extra_counter + 1; //Designate start idx for new entry

				//Encode a line and write it to disk.
				std::vector<unsigned char> encoded_line = huffmanEncoder.full_encode_line(line);
				binfile.write(&encoded_line);

				//Set prevLine
				prev_line = line;

			} else{
				//If we still have the same line, just append to it:
				std::vector<unsigned char> encoded_line = huffmanEncoder.full_encode_line(line);
				binfile.write(&encoded_line);
			}

		} catch (util::EndOfFileException e){
			std::cerr << "Reading phrase table finished, writing remaining files to disk." << std::endl;
			binfile.flush();
			break;
		}
	}

	serialize_table(mem, size, (basepath + "/probing_hash.dat").c_str());
	
	delete[] mem;

	//Write configfile
	std::ofstream configfile;
	configfile.open((basepath + "/config").c_str());
	configfile << uniq_entries << '\n' << longestchars;
	std::cerr << "Longest char sequence detected is: " << longestchars << std::endl;
	configfile.close();
}
