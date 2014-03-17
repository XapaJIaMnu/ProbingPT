#include "helpers/huffmanish.hh"

void printbits(unsigned char v) {
  int i; // for C89 compatability
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

int main(int argc, char* argv[]){
	HuffmanDecoder decoder(argv[1]); //Test initialization of huffman decoder. Argument is directory with maps.
	std::vector<unsigned char> num1 = vbyte_encode(0);
	std::vector<unsigned char> num2 = vbyte_encode(127);
	std::vector<unsigned char> num3 = vbyte_encode(128);
	std::vector<unsigned char> num4 = vbyte_encode(1544);
	std::vector<unsigned char> num5 = vbyte_encode(49802);
	std::vector<unsigned char> num6 = vbyte_encode(3235485);
	std::vector<unsigned char> big_vector; //Vector containing everything

	std::cout << "Number 1" << std:: endl;
	for (std::vector<unsigned char>::iterator it = num1.begin(); it != num1.end(); it++){
		printbits(*it);
		big_vector.push_back(*it);
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Number 2" << std:: endl;
	for (std::vector<unsigned char>::iterator it = num2.begin(); it != num2.end(); it++){
		printbits(*it);
		big_vector.push_back(*it);
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Number 3" << std:: endl;
	for (std::vector<unsigned char>::iterator it = num3.begin(); it != num3.end(); it++){
		printbits(*it);
		big_vector.push_back(*it);
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Number 4" << std:: endl;
	for (std::vector<unsigned char>::iterator it = num4.begin(); it != num4.end(); it++){
		printbits(*it);
		big_vector.push_back(*it);
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Number 5" << std:: endl;
	for (std::vector<unsigned char>::iterator it = num5.begin(); it != num5.end(); it++){
		printbits(*it);
		big_vector.push_back(*it);
		std::cout << std::endl;
	}

	for (std::vector<unsigned char>::iterator it = num6.begin(); it != num6.end(); it++){
		big_vector.push_back(*it);
	}


	std::cout << std::endl;

	std::cout << "Conversion back!" << std::endl;
	std::cout << bytes_to_int(num1) << std::endl;
	std::cout << bytes_to_int(num2) << std::endl;
	std::cout << bytes_to_int(num3) << std::endl;
	std::cout << bytes_to_int(num4) << std::endl;
	std::cout << bytes_to_int(num5) << std::endl;

	std::vector<unsigned int> decode_results = vbyte_decode(big_vector);

	for (std::vector<unsigned int>::iterator it = decode_results.begin(); it != decode_results.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::vector<int> pesho = {1,2,3};
	std::vector<int> kiro = {1,2,3};
	std::vector<int> giro = {2,1,3};
	std::vector<int> gosho = {1,2,3,4};

	bool z = pesho==kiro;

	std::cout << (pesho==kiro) << ' ' << (kiro == giro) << ' ' << (kiro == gosho) << std::endl;
}