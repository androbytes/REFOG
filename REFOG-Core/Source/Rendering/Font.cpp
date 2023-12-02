#include "REFOG/Rendering/Objects/Texture.h"
#include "REFOG/Rendering/Font.h"
#include "REFOG/Logger.h"
#include "glad/glad.h"
#include <limits>

namespace REFOG {
	FT_Library Font::m_Library = nullptr;
	unsigned int Font::m_FontCount = 0;

	Font::Font() {
		m_Characters = std::map<char, Character>();
		m_FontCount++;

		if (&m_Library == nullptr) return;
		if (FT_Init_FreeType(&m_Library)) {
			REFOG_ERROR("Failed to init FreeType");
			return;
		}
		else {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	Font::~Font() {
		m_FontCount--;
		if (m_FontCount == 0) {
			FT_Done_FreeType(m_Library);
		}
	}

	void Font::LoadFont(std::string filePath) {
		if (FT_New_Face(m_Library, filePath.c_str(), 0, &m_Face)) {
			REFOG_ERROR("Failed to load face");
			REFOG_TRACE("File: " + filePath);
			return;
		}

		FT_Set_Pixel_Sizes(m_Face, 0, 48);
	}

	void Font::LoadChars() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (char i = 0; i < std::numeric_limits<char>::max(); i++) {
			if (FT_Load_Char(m_Face, i, FT_LOAD_RENDER)) {
				REFOG_ERROR("Failed to load char.");
				REFOG_TRACE(i);
				continue;
			}

			FT_Bitmap& bitmap = m_Face->glyph->bitmap;
			Texture* texture = new Texture{ Texture2D };
			texture->SetTextureInfo(TextureWrapS, ClampToEdge);
			texture->SetTextureInfo(TextureWrapT, ClampToEdge);
			texture->SetTextureInfo(TextureMin, Linear);
			texture->SetTextureInfo(TextureMag, Linear);
			texture->LoadTexture("", {
				.InternalFormat = R8,
				.Format = R,
				.Custom = true,
				.Width = (GLsizei)bitmap.width,
				.Height = (GLsizei)bitmap.rows,
				.Data = bitmap.buffer,
				});
			
			m_Characters.insert(std::pair<char, Character>(i, Character {
				texture,
				{ bitmap.width, bitmap.rows },
				{ m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top },
				(unsigned int)m_Face->glyph->advance.x
				}));
		}

		FT_Done_Face(m_Face);
	}

	Character Font::GetChar(char character) {
		return m_Characters[character];
	}
}