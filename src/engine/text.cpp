#include <engine/text.h>

namespace text {
	IntMap<string> texts_en;
	IntMap<string> texts_pl;
	uint8_t activeLang = LANG_ENGLISH;
}

string s;

string* text::getText(uint32_t textID) {
	if (activeLang == LANG_ENGLISH) return texts_en.get(textID);
	else if (activeLang == LANG_POLISH) return texts_pl.get(textID);
	else {
		s.clear();
		s.add("TEXT_");
		s.add(textID);
		return &s;
	}
}

uint32_t text::utf8Convert(uint16_t* unicode_point, const char* text, uint32_t position) {
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
