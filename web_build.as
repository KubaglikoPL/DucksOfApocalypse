string cc = "mipsel-unknown-elf-gcc ";
string cxx = "mipsel-unknown-elf-g++ ";
string ld = "mipsel-unknown-elf-ld ";

string common_flags = "-O2 -g3 -s USE_GLFW=3 -s LEGACY_GL_EMULATION=1 -s GL_FFP_ONLY=1 -Iinclude -I3rd_party -s ASSERTIONS=1 -s GL_UNSAFE_OPTS=0 ";
string cflags = common_flags + "";
string linker_flags = common_flags + "-s WASM=0 --embed-file data.tar ";

array<string> object_files;

void compileCXX(string inputFile, string outputFile) {
  string command = "em++ " + cflags + inputFile + " -c -o web_build/" + outputFile;
  object_files.insertLast(outputFile);
  print("Compiling... " + inputFile + "\n");
  exec(command);
}

void link() {
  string command = "em++ " + linker_flags;
  for(int i = 0; i < object_files.length(); i++) {
    command = command + " web_build/" + object_files[i];
  }
  command = command + " -o web_build/web_build.html";
  print("Linking...");
  exec(command);
}

/*string msys_bin = "D:\\Dev\\msys2\\usr\\bin";
string msys_mingw_bin = "D:\\Dev\\msys2\\mingw64\\bin";
string mipsel_bin = "D:\\Dev\\PSX\\mipsel-unknown-elf\\bin";
string psx_tools = "D:\\Dev\\PSX\\psn00bsdk\\tools\\bin";*/

void main() {
  exec('cls');
  //exec('set PATH=%PATH%;' + msys_bin);
  //exec('set PATH=%PATH%;' + msys_mingw_bin);
  //exec('set PATH=%PATH%;' + mipsel_bin);
  //exec('set PATH=%PATH%;' + psx_tools);
  compileCXX("src/main.cpp", "main.o");
  compileCXX("src/map.cpp", "map.o");
  compileCXX("src/game_gui.cpp", "game_gui.o");
  //compileCXX("src/engine/file_loader_psx.cpp", "file_loader_psx.o");
  compileCXX("src/engine/img.cpp", "img.o");
  //compileCXX("src/engine/render_psx.cpp", "render_psx.o");
  compileCXX("src/engine/render.cpp", "render.o");
  compileCXX("src/engine/tar.cpp", "tar.o");
  compileCXX("src/engine/text.cpp", "text.o");
  compileCXX("src/engine/input.cpp", "input.o");
  compileCXX("src/engine/gui.cpp", "gui.o");
  link();
}
