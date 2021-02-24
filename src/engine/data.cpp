#include <engine/data.h>
#include <engine/file.h>
#include <engine/text.h>
#include <ini.h>
#include <CXXoverC/string.h>
#include <stdlib.h>

IntMap<UnitData> unitsData;
IntMap<ExtendedCharData> extendedChars;

uint32_t activeUnitID = 0;
uint32_t lastExtendedCharUnicodePoint = 0;

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
	if (!strcmp(section, "extendedChar")) {
		if (!strcmp(name, "unicodePoint")) {
			extendedChars.add(atoi(value), ExtendedCharData{});
			lastExtendedCharUnicodePoint = atoi(value);
		}
		if (!strcmp(name, "textureY")) extendedChars.get(lastExtendedCharUnicodePoint)->textureY = atoi(value);
		if (!strcmp(name, "textureX")) extendedChars.get(lastExtendedCharUnicodePoint)->textureX = atoi(value);
	}
	if (!strcmp(section, "unit")) {
		if (!strcmp(name, "ID")) {
			activeUnitID = atoi(value);
			unitsData.add(activeUnitID, UnitData{});
		}
		if (!strcmp(name, "name")) unitsData.get(activeUnitID)->textID = atoi(value);
		if (!strcmp(name, "maxHP")) unitsData.get(activeUnitID)->maxHP = atoi(value);
		if (!strcmp(name, "attackType")) unitsData.get(activeUnitID)->attackType = atoi(value);
		if (!strcmp(name, "minAttack")) unitsData.get(activeUnitID)->minAttack = atoi(value);
		if (!strcmp(name, "maxAttack")) unitsData.get(activeUnitID)->maxAttack = atoi(value);
		if (!strcmp(name, "type")) unitsData.get(activeUnitID)->type = atoi(value);
		if (!strcmp(name, "armor")) unitsData.get(activeUnitID)->armor = atoi(value);
		if (!strcmp(name, "range")) unitsData.get(activeUnitID)->range = atoi(value);
		if (!strcmp(name, "textureX")) unitsData.get(activeUnitID)->textureX = atoi(value);
		if (!strcmp(name, "textureY")) unitsData.get(activeUnitID)->textureY = atoi(value);
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
