#include <engine/data.h>
#include <engine/file.h>
#include <engine/text.h>
#include <ini.h>
#include <CXXoverC/string.h>
#include <stdlib.h>

int ini_parse(void* user, const char* section, const char* name, const char* value) {
	if (!strcmp(section, "textsEN")) {
		string str(value);
		uint32_t key = atoi(name);
		text::texts_en.add(key, str);
	}
	if (!strcmp(section, "textsPL")) {
		string str(value);
		uint32_t key = atoi(name);
		text::texts_pl.add(key, str);
	}
	//printf("%s\n", section);
	//printf("%s\n", name);
	//printf("%s\n", value);
	return 1;
}

void loadData(const char* filepath) {
	uint32_t fileLen;
	uint8_t* fileData = loadFile(filepath, fileLen);
	if (fileData) {
		string ini_str;
		ini_str.add((const char*)fileData, fileLen);
		ini_parse_string(ini_str.data(), &ini_parse, nullptr);
	}
}
