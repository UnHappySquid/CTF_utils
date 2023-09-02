#include <iostream>
#include <fstream>

using namespace std;

const unsigned int BITMAPCOREHEADER_SIZE = 12;
const unsigned int OS21XBITMAPHEADER_SIZE = 12;
const unsigned int OS22XBITMAPHEADER_SIZE = 16;
const unsigned int BITMAPINFOHEADER_SIZE = 40;
const unsigned int BITMAPV2INFOHEADER_SIZE = 52;
const unsigned int BITMAPV3INFOHEADER_SIZE = 56;
// This one is weird
//const unsigned int OS22XBITMAPHEADER_SIZE = 64;
const unsigned int BITMAPV4HEADER_SIZE = 108;
const unsigned int BITMAPV5HEADER_SIZE = 124;

const string size_wiki_link = "https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header)";

struct BmpFile {
	struct FileHeader {
		unsigned short int signature;
		unsigned int file_size;
		unsigned short int reserved_one;
		unsigned short int reserved_two;
		unsigned int offset;
	} file_header;

	struct BodyInfoHeader {
		unsigned int header_size;	
		
	} body_header;

	BmpFile(istream& ifs) {
		// File Header
		file_header.signature = read_usi(ifs); 
		file_header.file_size = read_ui(ifs);		
		file_header.reserved_one = read_usi(ifs);
		file_header.reserved_two = read_usi(ifs);
		file_header.offset = read_ui(ifs);
		
		// Body Header
		unsigned int tmp_size =
		       	body_header.header_size = read_ui(ifs);
		if (tmp_size != BITMAPCOREHEADER_SIZE &&
				tmp_size != OS21XBITMAPHEADER_SIZE &&
				tmp_size != OS22XBITMAPHEADER_SIZE &&
		  		tmp_size != BITMAPINFOHEADER_SIZE &&
		  		tmp_size != BITMAPV2INFOHEADER_SIZE &&
		  		tmp_size != BITMAPV3INFOHEADER_SIZE &&
		 		tmp_size != BITMAPV4HEADER_SIZE &&
			       	tmp_size != BITMAPV5HEADER_SIZE 	
		   ) {
			cout << "\033[1;31m[WARNING]\033[1;0m Body info header size is unconventional" << endl;
			cout << "\033[1;31m[WARNING]\033[1;0m body info size is : " << tmp_size << endl;
			cout << "\033[1;31m[WARNING]\033[1;0m refer to " << size_wiki_link<< " for more info." << endl;
		}
	}

private:
	unsigned short int read_usi(istream& ifs) {
		
		// used to read unsigned short ints
		unsigned char usi[2];
		ifs.read((char*) usi, 2);
		return *((unsigned short int*) usi);	
	}

	unsigned int read_ui(istream& ifs) {	
		// used to read unsigned ints
		unsigned char ui[4];
		ifs.read((char*) ui, 4);
		return *((unsigned int*) ui);
	}

};

void usage(string prog_name) {
	cout << "Wrong number of arguments" << endl;
	cout << "Usage : " << prog_name << " <bitmap_file_path>" << endl;
}

int main(int argn, char** args) {
	if (sizeof(unsigned short int) != 2 ||
			sizeof(unsigned int) != 4) {
		cout << "The type sizes aren't the same as what the program was written with." << endl;
		exit(2);
	}
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
	BmpFile file{ifs};
	cout << hex << file.file_header.signature << endl;
	cout << dec << file.file_header.file_size << endl;
	cout << hex << file.file_header.offset << endl;


	cout << "body info size is : " << dec << file.body_header.header_size<< endl;
}
