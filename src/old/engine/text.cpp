#include <engine/text.h>
#include <malloc.h>
#include <stdlib.h>
#include <engine/tar.h>
#include <data/font_data.h>

int utf8_convert(uint16_t* unicode_point, const char* text, int position) {
	unsigned char c0 = text[position];
	if (c0 & 128) {
		//Two byte character
		unsigned char b0 = c0 & 31;
		unsigned char c1 = text[position + 1];
		unsigned char b1 = c1 & 63;
		(*unicode_point) = (b0 << 6) | b1;
		return 2;
	}
	else {
		//One byte character
		(*unicode_point) = c0;
		return 1;
	}
}

struct extended_char_width {
	uint16_t unicode_code;
	unsigned char char_width;
};

typedef struct extended_char_width extended_char_width_t;

extended_char_width* extended_chars_width;
int extended_chars_width_number = 0;

int get_char_width(uint16_t unicodeCode) {
	if (unicodeCode < 128) {
		return base_chars_width[unicodeCode];
	}
	else {
		for (int i = 0; i < extended_chars_width_number; i++) {
			if (extended_chars_width[i].unicode_code == unicodeCode) {
				return extended_chars_width[i].char_width;
			}
		}
	}

	return 8;
}

/*void load_font_data(const char* filename) {
	int size;
	uint8_t* fileData = load_tar_entry(filename, &size);

	if (fileData != NULL) {
		tinyxml2::XMLDocument xmlDoc;
		if (xmlDoc.Parse((const char*)fileData, size) == tinyxml2::XML_SUCCESS) {
			tinyxml2::XMLElement* pFirstNode = xmlDoc.FirstChildElement();

			tinyxml2::XMLElement* pNode = pFirstNode;

			while (pNode) {
				if (!strcmp(pNode->Name(), "CharWidth")) {
					int charUnicodeCode = pNode->IntAttribute("char", 0);
					if (charUnicodeCode < 128) {
						base_chars_width[charUnicodeCode] = pNode->IntAttribute("width", 8);
					}
				}

				pNode = pNode->NextSiblingElement();
			}
		}
	}
}*/
