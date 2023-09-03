#include <fstream>
#include <iostream>

using namespace std;

const unsigned int BITMAPCOREHEADER_SIZE = 12;
const unsigned int OS21XBITMAPHEADER_SIZE = 12;
const unsigned int OS22XBITMAPHEADER_SIZE = 16;
const unsigned int BITMAPINFOHEADER_SIZE = 40;
const unsigned int BITMAPV2INFOHEADER_SIZE = 52;
const unsigned int BITMAPV3INFOHEADER_SIZE = 56;
// This one is weird
// const unsigned int OS22XBITMAPHEADER_SIZE = 64;
const unsigned int BITMAPV4HEADER_SIZE = 108;
const unsigned int BITMAPV5HEADER_SIZE = 124;

const string size_wiki_link =
    "https://en.wikipedia.org/wiki/"
    "BMP_file_format#DIB_header_(bitmap_information_header)";

// For simplicity, this class will assume BITMAPINFOHEADER_SIZE for
// BodyInfoHeader
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
    int width;
    int height;
    unsigned short int color_planes;
    unsigned short int color_depth;
    unsigned int holder;
  } body_header;

  BmpFile(istream &ifs) {
    // File Header
    file_header.signature = read_usi(ifs);
    file_header.file_size = read_ui(ifs);
    file_header.reserved_one = read_usi(ifs);
    file_header.reserved_two = read_usi(ifs);
    file_header.offset = read_ui(ifs);

    // Body Header
    unsigned int tmp_size = body_header.header_size = read_ui(ifs);

    // making sure size makes sense
    if (tmp_size != BITMAPCOREHEADER_SIZE &&
        tmp_size != OS21XBITMAPHEADER_SIZE &&
        tmp_size != OS22XBITMAPHEADER_SIZE &&
        tmp_size != BITMAPINFOHEADER_SIZE &&
        tmp_size != BITMAPV2INFOHEADER_SIZE &&
        tmp_size != BITMAPV3INFOHEADER_SIZE &&
        tmp_size != BITMAPV4HEADER_SIZE && tmp_size != BITMAPV5HEADER_SIZE) {
      cout << string(90, '-') << endl;
      cout << red_coat("[WARNING]")
           << " Body info header size is unconventional" << endl;
      cout << red_coat("[WARNING]") << " body info size is : " << tmp_size
           << endl;
      cout << red_coat("[WARNING]") << " refer to " << size_wiki_link
           << " for more info." << endl;
      cout << string(90, '-') << endl;
    }

    if (tmp_size != BITMAPINFOHEADER_SIZE) {
      cout << red_coat("[WARNING]")
           << " This program was written to extract information from a "
              "BITMAPINFOHEADER body header"
           << endl;
      cout << red_coat("[WARNING]") << " this bmp file has a header size of "
           << tmp_size << " instead of 40" << endl;
    }
    body_header.width = read_i(ifs);
    body_header.height = read_i(ifs);
    unsigned short int tmp_planes = body_header.color_planes = read_usi(ifs);
    if (tmp_planes != 1) {
      cout << "-" << endl;
      cout << red_coat("[WARNING]") << " Planes should be set to 1 but isn't";
    }
    body_header.color_depth = read_usi(ifs);
    body_header.holder = read_ui(ifs);
  }

private:
  string red_coat(string s) { return "\033[1;31m" + s + "\033[1;0m"; }

  unsigned short int read_usi(istream &ifs) {

    // used to read unsigned short ints
    unsigned char usi[2];
    ifs.read((char *)usi, 2);
    return *((unsigned short int *)usi);
  }

  unsigned int read_ui(istream &ifs) {
    // used to read unsigned ints
    unsigned char ui[4];
    ifs.read((char *)ui, 4);
    return *((unsigned int *)ui);
  }

  int read_i(istream &ifs) {
    // used to read unsigned ints
    unsigned char i[4];
    ifs.read((char *)i, 4);
    return *((int *)i);
  }
};

void usage(string prog_name) {
  cout << "Wrong number of arguments" << endl;
  cout << "Usage : " << prog_name << " <bitmap_file_path>" << endl;
}

int main(int argn, char **args) {
  if (sizeof(unsigned short int) != 2 || sizeof(unsigned int) != 4 ||
      sizeof(int) != 4) {
    cout << "The type sizes don't comply with the programs assumptions about "
            "them."
         << endl;
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

  cout << "body info size is : " << dec << file.body_header.header_size << endl;

  cout << "image width : " << dec << file.body_header.width << endl;
  cout << "image height : " << dec << file.body_header.height << endl;
  cout << "image planes : " << dec << file.body_header.color_planes << endl;
  cout << "image color depth : " << dec << file.body_header.color_depth << endl;
}
