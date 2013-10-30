#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

int main(){
	std::vector<int> pesho(5);

	pesho[0] = 1;
	pesho[1] = 2;
	pesho[2] = 3;
	pesho[3] = 8;
	pesho[4] = 22;

	//write
	std::ofstream os ("data.dat", std::ios::binary);

    int size1 = pesho.size();
    os.write((const char*)&size1, 4);
    os.write((const char*)&pesho[0], size1 * sizeof(int));
    os.close();

    //read
}