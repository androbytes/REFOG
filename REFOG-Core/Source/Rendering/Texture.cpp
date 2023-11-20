#include "REFOG/Rendering/Texture.h"
#include "stb_image.h"
#include "REFOG/Logger.h"

namespace REFOG {
	Texture::Texture(TextureType type) {
		glCreateTextures(type, 1, &m_Texture);
		m_Type = type;
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_Texture);
	}

	// For setting units, you can have a static variable that resets every frame
	// that stores the current bound unit count (incremented by function).
	void Texture::SetUnit(GLuint unit) {
		glBindTextureUnit(unit, m_Texture);
	}

	void Texture::SetTextureInfo(TextureParamName name, TextureParamValue value) {
		glTextureParameteri(m_Texture, name, value);
	}

	void Texture::LoadTexture(std::string_view file, TextureInfo info) {
		int x, y, channels;
		unsigned char* pixels = nullptr;
		if (info.Custom) {
			pixels = (unsigned char*)info.Data;
			x = info.Width;
			y = info.Height;
		} else {
			pixels = stbi_load(file.data(), &x, &y, &channels, 0);
		}

		if (!pixels) {
			REFOG_ERROR("Failed to load file. Filename: " + std::string(file));
			return;
		}
		
		switch (m_Type)
		{
		case Texture1D:
		case Texture1DArray:
			glTextureStorage1D(m_Texture, 1, info.InternalFormat, x);
			glTextureSubImage1D(m_Texture, info.MipMap, 0, x, info.Format, GL_UNSIGNED_BYTE, pixels);
			glGenerateTextureMipmap(m_Texture);
			REFOG_INFO("Generated 1D Texture");
			break;
		case Texture2D:
		case Texture2DArray:
			glTextureStorage2D(m_Texture, 1, info.InternalFormat, x, y);
			glTextureSubImage2D(m_Texture, info.MipMap, 0, 0, x, y, info.Format, GL_UNSIGNED_BYTE, pixels);
			glGenerateTextureMipmap(m_Texture);
			REFOG_INFO("Generated 2D Texture");
			break;
		/*case Texture2DMultisample:
		case Texture2DMultisampleArray:
			glTexImage2DMultisample(m_Type, info.MipMap, info.Format, x, y, info.FixedSampleLocations);
			glGenerateMipmap(m_Type);
			REFOG_INFO("Generated 2D Multisample Texture");
			break;*/
		case Texture3D:
			glTextureStorage3D(m_Texture, 1, info.InternalFormat, x, y, info.Depth);
			glTextureSubImage3D(m_Texture, info.MipMap, 0, 0, 0, x, y, info.Depth, info.Format, GL_UNSIGNED_BYTE, pixels);
			glGenerateTextureMipmap(m_Texture);
			REFOG_INFO("Generated 3D Texture");
			break;
		default:
			REFOG_WARN("Unknown/Unsupported texture type.");
			break;
		}

		if (info.Custom) return;
		stbi_image_free(pixels);
	}

	GLuint Texture::GetID() {
		return m_Texture;
	}
}