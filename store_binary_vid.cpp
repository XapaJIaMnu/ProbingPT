#include "util/usage.hh"

#include <ctime> //for timing.
#include <chrono>
#include "helpers/storing.hh"



int main(int argc, char* argv[]){

	//Time everything
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();
	const char * is_reordering = "false";


	if (!(argc == 5 || argc == 4)) {
		// Tell the user how to run the program
		std::cerr << "Provided " << argc << " arguments, needed 4 or 5." << std::endl;
		std::cerr << "Usage: " << argv[0] << " path_to_phrasetable output_dir num_scores is_reordering" << std::endl;
		std::cerr << "is_reordering should be either true or false, but it is currently a stub feature." << std::endl;
		//std::cerr << "Usage: " << argv[0] << " path_to_phrasetable number_of_uniq_lines output_bin_file output_hash_table output_vocab_id" << std::endl;
		return 1;
	}

	if (argc == 5) {
		is_reordering = argv[4];
	}

	createProbingPT(argv[1], argv[2], argv[3], is_reordering);

	//End timing
	std::clock_t c_end = std::clock();
	auto t_end = std::chrono::high_resolution_clock::now();

	//Print timing results
	std::cout << "CPU time used: "<< 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC<< " ms\n";
	std::cout << "Real time passed: "<< std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count()<< " ms\n";


	util::PrintUsage(std::cout);
	return 1;
}

