#include <iostream>
#include <fstream>

using namespace std;

struct BmpStruct {
	struct FileHeader {
		unsigned short int signature[2];
		unsigned int file_size;
		unsigned short int reserved_one;
		unsigned short int reserved_two;
		unsigned int offset_bit;
	}
	struct BodyInfoHeader {
		unsigned int header_size;	
	}

}

void usage(string prog_name) {
	cout << "Wrong number of arguments" << endl;
	cout << "Usage : " << prog_name << " <bitmap_file_path>" << endl;
}

int main(int argn, char** args) {
	if (argn != 2) {
		usage(args[0]);
		exit(1);
	}
	string file_path = args[1];
	ifstream ifs{file_path, ios::binary};
	if (!ifs) {
		cout << "Cannot open file : " << file_path << endl;
		exit(1);
	}
	for (unsigned char c; ifs >> c;) {
		cout << hex << c << dec; 
	}
}
