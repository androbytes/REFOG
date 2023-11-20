#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "glm/vec2.hpp"

#include <string>
#include <map>

namespace REFOG {
	extern class Texture;
	struct Character {
		Texture*     Texture;
		glm::ivec2   Size;
		glm::ivec2   Bearing;
		unsigned int Advance;
	};

	class Font {
	private:
		static FT_Library m_Library;
		static unsigned int m_FontCount;
		FT_Face m_Face;

		std::map<char, Character> m_Characters;
	public:
		// When a font is created, blending is enabled
		Font();
		virtual ~Font();

		void LoadFont(std::string filePath);
		void LoadChars();

		Character GetChar(char);
	};
}
