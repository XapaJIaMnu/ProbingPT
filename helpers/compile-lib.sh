c++ -O3 -c -I.. -I../boost/include hash.cpp line_splitter.cpp probing_hash_utils.cpp quering.cpp vocabid.cpp storing.cpp

ar rvs libprobingPT.a hash.o line_splitter.o probing_hash_utils.o quering.o vocabid.o storing.o
