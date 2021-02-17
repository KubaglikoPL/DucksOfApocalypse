#include <engine/text.h>

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
