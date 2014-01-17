#include "helpers/line_splitter.hh" 

bool probabilitiesTest(){
	StringPiece line1 = StringPiece("0.536553 0.75961 0.634108 0.532927 2.718");
	StringPiece line2 = StringPiece("1.42081e-05 3.91895e-09 0.0738539 0.749514 2.718");

	std::vector<double> pesho;
	bool peshobool = false;
	bool kirobool = false;
	std::vector<double> kiro;

	pesho = splitProbabilities(line1);
	kiro = splitProbabilities(line2);

	if (pesho[0] == 0.536553 && pesho[1] == 0.75961 && pesho[2] == 0.634108 && pesho[3] == 0.532927 && pesho[4] == 2.718 && pesho.size() == 5) {
		peshobool = true;
	} else {
		std::cout << "Processed: " << pesho[0] << " " << pesho[1] << " " << pesho[2] << " " << pesho[3] << " " << pesho[4] << std::endl;
		std::cout << "Size is: " << pesho.size() << " Expected 5." << std::endl;
		std::cout << "Expected: " << "0.536553 0.75961 0.634108 0.532927 2.718" << std::endl;
	}

	if (kiro[0] == 1.42081e-05 && kiro[1] == 3.91895e-09 && kiro[2] == 0.0738539 && kiro[3] == 0.749514 && kiro[4] == 2.718 && kiro.size() == 5) {
		kirobool = true;
	} else {
		std::cout << "Processed: " << kiro[0] << " " << kiro[1] << " " << kiro[2] << " " << kiro[3] << " " << kiro[4] << std::endl;
		std::cout << "Size is: " << kiro.size() << " Expected 5." << std::endl;
		std::cout << "Expected: " << "1.42081e-05 3.91895e-09 0.0738539 0.749514 2.718" << std::endl;
	}

	return (peshobool && kirobool);
}

bool wordAll1test(){
	StringPiece line1 = StringPiece("2-0 3-1 4-2 5-2");
	StringPiece line2 = StringPiece("0-0 1-1 2-2 3-3 4-3 6-4 5-5");

	std::vector<int> pesho;
	bool peshobool = false;
	bool kirobool = false;
	std::vector<int> kiro;

	pesho = splitWordAll1(line1);
	kiro = splitWordAll1(line2);

	if (pesho[0] == 2 && pesho[1] == 0 && pesho[2] == 3 && pesho[3] == 1 && pesho[4] == 4 
		&& pesho[5] == 2 && pesho[6] == 5 && pesho[7] == 2 && pesho.size() == 8) {
		peshobool = true;
	} else {
		std::cout << "Processed: " << pesho[0] << "-" << pesho[1] << " " << pesho[2] << "-" << pesho[3] << " "
		<< pesho[4] << "-" << pesho[5] << " " << pesho[6] << "-" << pesho[7] << std::endl;
		std::cout << "Size is: " << pesho.size() << " Expected: 8." << std::endl;
		std::cout << "Expected: " << "2-0 3-1 4-2 5-2" << std::endl;
	}

	if (kiro[0] == 0 && kiro[1] == 0 && kiro[2] == 1 && kiro[3] == 1 && kiro[4] == 2 && kiro[5] == 2
		&& kiro[6] == 3 && kiro[7] == 3 && kiro[8] == 4 && kiro[9] == 3 && kiro[10] == 6 && kiro[11] == 4
		&& kiro[12] == 5 && kiro[13] == 5 && kiro.size() == 14){
		kirobool = true;
	} else {
		std::cout << "Processed: " << kiro[0] << "-" << kiro[1] << " " << kiro[2] << "-" << kiro[3] << " "
		<< kiro[4] << "-" << kiro[5] << " " << kiro[6] << "-" << kiro[7] << " " << kiro[8] << "-" << kiro[9]
		<< " " << kiro[10] << "-" << kiro[11] << " " << kiro[12] << "-" << kiro[13] << std::endl;
		std::cout << "Size is: " << kiro.size() << " Expected: 14" << std::endl;
		std::cout << "Expected: " << "0-0 1-1 2-2 3-3 4-3 6-4 5-5" << std::endl;
	}

	return (peshobool && kirobool);
}

bool wordAll2test(){
	StringPiece line1 = StringPiece("4 9 1");
	StringPiece line2 = StringPiece("3255 9 1");

	std::vector<int> pesho;
	bool peshobool = false;
	bool kirobool = false;
	std::vector<int> kiro;

	pesho = splitWordAll2(line1);
	kiro = splitWordAll2(line2);

	if (pesho[0] == 4 && pesho[1] == 9 && pesho[2] == 1 && pesho.size() == 3){
		peshobool = true;
	} else {
		std::cout << "Processed: " << pesho[0] << " " << pesho[1] << " " << pesho[2] << std::endl;
		std::cout << "Size: " << pesho.size() << " Expected: 3" << std::endl;
		std::cout << "Expected: " << "4 9 1" << std::endl;
	}

	if (kiro[0] == 3255 && kiro[1] == 9 && kiro[2] == 1 && kiro.size() == 3){
		kirobool = true;
	} else {
		std::cout << "Processed: " << kiro[0] << " " << kiro[1] << " " << kiro[2] << std::endl;
		std::cout << "Size: " << kiro.size() << " Expected: 3" << std::endl;
		std::cout << "Expected: " << "3255 9 1" << std::endl;
	}

	return (peshobool && kirobool);

}

bool test_tokenization(){
	StringPiece line1 = StringPiece("! ! ! ! ||| ! ! ! ! ||| 0.0804289 0.141656 0.0804289 0.443409 2.718 ||| 0-0 1-1 2-2 3-3 ||| 1 1 1");
	StringPiece line2 = StringPiece("! ! ! ) , has ||| ! ! ! ) - , a ||| 0.0804289 0.0257627 0.0804289 0.00146736 2.718 ||| 0-0 1-1 2-2 3-3 4-4 4-5 5-6 ||| 1 1 1");
	StringPiece line3 = StringPiece("! ! ! ) , ||| ! ! ! ) - , ||| 0.0804289 0.075225 0.0804289 0.00310345 2.718 ||| 0-0 1-1 2-2 3-3 4-4 4-5 ||| 1 1 1");
	StringPiece line4 = StringPiece("! ! ! ) ||| ! ! ! ) . ||| 0.0804289 0.177547 0.0268096 0.000872597 2.718 ||| 0-0 1-1 2-2 3-3 ||| 1 3 1");

	line_text output1 = splitLine(line1);
	line_text output2 = splitLine(line2);
	line_text output3 = splitLine(line3);
	line_text output4 = splitLine(line4);

	bool test1 = output1.prob == StringPiece("0.0804289 0.141656 0.0804289 0.443409 2.718");
	bool test2 = output2.word_all1 == StringPiece("0-0 1-1 2-2 3-3 4-4 4-5 5-6");
	bool test3 = output2.target_phrase == StringPiece("! ! ! ) - , a");
	bool test4 = output3.source_phrase == StringPiece("! ! ! ) ,");
	bool test5 = output4.word_all2 == StringPiece("1 3 1");

	//std::cout << test1 << " " << test2 << " " << test3 << " " << test4 << std::endl;

	return (test1 && test2 && test3 && test4 && test5);

}

int main(){
	if (probabilitiesTest() && wordAll1test() && wordAll2test() && test_tokenization()){
		std::cout << "All tests pass!" << std::endl;
	} else {
		std::cout << "Failiure in some tests!" << std::endl;
	}

	return 1;
}