#include <stdio.h>
#include <fstream>
#include <iostream>  

#include "helpers/hash.hh"
#include "helpers/line_splitter.hh"
#include "helpers/probing_hash_utils.hh"
#include "helpers/vocabid.hh"
#include <sys/stat.h> //mkdir

#include "util/file_piece.hh"
#include "util/file.hh"

void createProbingPT(const char * phrasetable_path, const char * target_path);