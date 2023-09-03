#include <fstream>
#include <iostream>

using namespace std;

const int das_reps = 90;

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
    unsigned int compression;
    unsigned int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    unsigned int colors_used;
    unsigned int colors_importance;
  } body_header;

  BmpFile(string file_path) :file_offset{0}, ifs{file_path, ios::binary} {
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
      cerr << string(das_reps, '-') << endl;
      cerr << red_coat("[WARNING]")
           << " Body info header size is unconventional" << endl;
      cerr << red_coat("[WARNING]") << " body info size is : " << tmp_size
           << endl;
      cerr << red_coat("[WARNING]") << " refer to " << size_wiki_link
           << " for more info." << endl;
      cerr << string(das_reps, '-') << endl;
    }

    if (tmp_size != BITMAPINFOHEADER_SIZE) {
      cerr << string(das_reps, '-') << endl;
      cerr << red_coat("[WARNING]")
           << " This program was written to extract information from a "
              "BITMAPINFOHEADER body header"
           << endl;
      cerr << red_coat("[WARNING]") << " this bmp file has a header size of "
           << tmp_size << " instead of 40" << endl;
      cerr << string(das_reps, '-') << endl;
    }

    body_header.width = read_i(ifs);
    body_header.height = read_i(ifs);

    unsigned short int tmp_planes = body_header.color_planes = read_usi(ifs);
    if (tmp_planes != 1) {
      cerr << string(das_reps, '-') << endl;
      cerr << red_coat("[WARNING]") << " Planes should be set to 1 but isn't";
      cerr << string(das_reps, '-') << endl;
    }

    body_header.color_depth = read_usi(ifs);
    body_header.compression = read_ui(ifs);
    body_header.image_size = read_ui(ifs);
    body_header.x_pixels_per_meter = read_i(ifs);
    body_header.y_pixels_per_meter = read_i(ifs);
    body_header.colors_used = read_ui(ifs);
    body_header.colors_importance = read_ui(ifs);
  }

  void write_to_file(string destination_path) {
    ofstream ofs{destination_path, ios::binary};
    if (!ofs) {
      cerr << red_coat("[ERROR]") << " Unable to open file " << destination_path
           << endl;
      return;
    }
    write_usi(ofs, file_header.signature);
    write_ui(ofs, file_header.file_size);
    write_usi(ofs, file_header.reserved_one);
    write_usi(ofs, file_header.reserved_two);
    write_ui(ofs, file_header.offset);

    write_ui(ofs, body_header.header_size);
    write_i(ofs, body_header.width);
    write_i(ofs, body_header.height);
    write_usi(ofs, body_header.color_planes);
    write_usi(ofs, body_header.color_depth);
    write_ui(ofs, body_header.compression);
    write_ui(ofs, body_header.image_size);
    write_i(ofs, body_header.x_pixels_per_meter);
    write_i(ofs, body_header.y_pixels_per_meter);
    write_ui(ofs, body_header.colors_used);
    write_ui(ofs, body_header.colors_importance);

    for (char c; ifs.read(&c, 1);) { 
        ofs.write(&c, 1);
    }
  }

  unsigned int get_current_offset() { return file_offset; }

private:
  unsigned int file_offset ;
  ifstream ifs;
  string red_coat(string s) { return "\033[1;31m" + s + "\033[1;0m"; }

  unsigned short int read_usi(istream &ifs) {

    ifs.seekg(file_offset);
    // used to read unsigned short ints
    unsigned char usi[2];
    ifs.read((char *)usi, 2);
    file_offset += 2;
    return *((unsigned short int *)usi);
  }

  unsigned int read_ui(istream &ifs) {
    ifs.seekg(file_offset);
    // used to read unsigned ints
    unsigned char ui[4];
    ifs.read((char *)ui, 4);
    file_offset += 4;
    return *((unsigned int *)ui);
  }

  int read_i(istream &ifs) {
    ifs.seekg(file_offset);
    // used to read unsigned ints
    unsigned char i[4];
    ifs.read((char *)i, 4);
    file_offset += 4;
    return *((int *)i);
  }

  void write_usi(ostream &ofs, unsigned short int usi) {
    ofs.write((char *)&usi, 2);
  }

  void write_ui(ostream &ofs, unsigned int ui) { ofs.write((char *)&ui, 4); }

  void write_i(ostream &ofs, int i) { ofs.write((char *)&i, 4); }

  void set_offset(unsigned int off) { file_offset = off; }
};

void usage(string prog_name) {
  cout << "Wrong number of arguments" << endl;
  cout << "Usage : " << prog_name << " <bitmap_file_path>" << endl;
}

// this is how i used this class 
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
  ifs.close();
  BmpFile file{args[1]};
  cout << hex << file.file_header.signature << endl;
  cout << "File size is : "<< dec << file.file_header.file_size << endl;
  cout << "Offset to color indices table : " << hex << file.file_header.offset
       << endl;

  cout << "body info size is : " << dec << file.body_header.header_size << endl;

  cout << "image width : " << dec << file.body_header.width << endl;
  cout << "image height : " << dec << file.body_header.height << endl;
  cout << "image planes : " << dec << file.body_header.color_planes << endl;
  cout << "image color depth : " << dec << file.body_header.color_depth << endl;
  cout << "color pallet size : " << dec << file.body_header.colors_used << endl;
  cout << "color importance : " << dec << file.body_header.colors_importance
       << endl;
  cout << "image size is : " << dec << file.body_header.image_size << endl;
  cout << "writtin to out.bmp" << endl;

  // This is part of the code i need to fix a bmp file for a ctf
  file.file_header.offset = 40 + 14;
  file.body_header.header_size = 40;
  file.body_header.height *= 2.8;
  file.write_to_file("./out.bmp");
  system("fim out.bmp");
}
