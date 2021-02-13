string psx_library_dir = "-Lpsx_deps";
string psx_header_dir = "-Ipsx_deps -Iinclude -I3rd_party ";
string gcc_base = "D:/Dev/PSX/mipsel-unknown-elf";
string gcc_bin = gcc_base + "/bin/";
string libraries = " -lpsxcd -lpsxgpu -lpsxgte -lpsxspu -lpsxsio -lpsxetc -lpsxapi -lc  ";

string cc = "mipsel-unknown-elf-gcc ";
string cxx = "mipsel-unknown-elf-g++ ";
string ld = "mipsel-unknown-elf-ld ";

string cflags = "-g -O2 -fno-builtin -fdata-sections -ffunction-sections -D PSX_BUILD";
string cppflags = cflags + " -fno-builtin -fno-rtti -fno-exceptions";
string linker_flags = "-g -Ttext=0x80010000 -gc-sections -T " + gcc_base + "/mipsel-unknown-elf/lib/ldscripts/elf32elmip.x ";

string out_dir = "psx_build";
string elf_output = "psx_game.elf";

array<string> object_files;

void compileCXX(string inputFile, string outputFile) {
  string command = cxx + " " + cppflags + " " + psx_header_dir + "-c " + inputFile + " -o " + out_dir + "\\" + outputFile;
  //print(command);
  //print("\n");
  object_files.insertLast(outputFile);
  exec(command);
}

void link() {
  string command = ld + linker_flags + psx_library_dir;
  for(int i = 0; i < object_files.length(); i++) {
    command = command + " " + out_dir + "\\" + object_files[i];
  }
  command = command + libraries;
  command = command + "-o " + out_dir + "\\" + elf_output;
  //print(command);
  //print('\\n');
  exec(command);

  string command2 = "elf2x -q " + out_dir + "\\" + elf_output;
  exec(command2);

  string command3 = "mkpsxiso psx_iso.xml";
  exec(command3);
}

string msys_bin = "D:\\Dev\\msys2\\usr\\bin";
string msys_mingw_bin = "D:\\Dev\\msys2\\mingw64\\bin";
string mipsel_bin = "D:\\Dev\\PSX\\mipsel-unknown-elf\\bin";
string psx_tools = "D:\\Dev\\PSX\\psn00bsdk\\tools\\bin";

void main() {
  exec('cls');
  exec('set PATH=%PATH%;' + msys_bin);
  exec('set PATH=%PATH%;' + msys_mingw_bin);
  exec('set PATH=%PATH%;' + mipsel_bin);
  exec('set PATH=%PATH%;' + psx_tools);
  compileCXX("src/main.cpp", "main.o");
  compileCXX("src/map.cpp", "map.o");
  compileCXX("src/game_gui.cpp", "game_gui.o");
  compileCXX("src/engine/file_loader_psx.cpp", "file_loader_psx.o");
  compileCXX("src/engine/img.cpp", "img.o");
  compileCXX("src/engine/render_psx.cpp", "render_psx.o");
  compileCXX("src/engine/render.cpp", "render.o");
  compileCXX("src/engine/tar.cpp", "tar.o");
  compileCXX("src/engine/text.cpp", "text.o");
  compileCXX("src/engine/input.cpp", "input.o");
  compileCXX("src/engine/gui.cpp", "gui.o");
  link();
}
