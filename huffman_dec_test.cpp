#include "helpers/huffmanish.hh"

void printbits(unsigned char v) {
  int i; // for C89 compatability
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

bool vbyte_test(){
	std::vector<unsigned int> pesho = {1,2423,34,23231,0,4333445,231,158223,0,199,17};
	std::vector<unsigned char> vbyte_encoded = vbyte_encode_line(pesho);
	std::vector<unsigned int> vbyte_decoded = vbyte_decode_line(vbyte_encoded);

	return pesho == vbyte_decoded;
}

int main(int argc, char* argv[]){
	HuffmanDecoder decoder(argv[1]); //Test initialization of huffman decoder. Argument is directory with maps.
	std::cout << "Result from vbyte test is: " << vbyte_test() << std::endl;
}