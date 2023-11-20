#pragma once
#include "glad/glad.h"
#include <string_view>

namespace REFOG {
	enum TextureType {
		Texture1D = GL_TEXTURE_1D,
		Texture1DArray = GL_TEXTURE_1D_ARRAY,
		Texture2D = GL_TEXTURE_2D,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		Texture3D = GL_TEXTURE_3D,
	};

	enum TextureFormat {
		R = GL_RED,
		R8 = GL_R8,
		R8UI = GL_R8UI,
		RInt = GL_RED_INTEGER,
		RG = GL_RG8UI,
		RGB = GL_RGB8UI,
		RGBInt = GL_RGB_INTEGER,
		//RGBA = GL_RGBA,
		RGBA = GL_RGBA8UI,
		RGBAInt = GL_RGBA_INTEGER,
		RGB10 = GL_RGB10,
		//SRGB = GL_SRGB,
		SRGB = GL_SRGB8,
	};
	
	enum TextureParamName {
		TextureWrapR = GL_TEXTURE_WRAP_R,
		TextureWrapS = GL_TEXTURE_WRAP_S,
		TextureWrapT = GL_TEXTURE_WRAP_T,
		TextureMin = GL_TEXTURE_MIN_FILTER,
		TextureMag = GL_TEXTURE_MAG_FILTER,
	};

	enum TextureParamValue {
		Nearest = GL_NEAREST,
		Repeat = GL_REPEAT,
		Linear = GL_LINEAR,
		LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR,
		ClampToEdge = GL_CLAMP_TO_EDGE,
	};

	struct TextureInfo {
		TextureFormat InternalFormat;
		TextureFormat Format = RGBAInt;
		GLint MipMap = 0; // Used as samples for multisample
		GLint Depth = 0;
		GLint Border = 0;
		GLboolean FixedSampleLocations = GL_FALSE;

		bool Custom      = false;
		GLsizei Width    = 0;
		GLsizei Height   = 0;
		const void* Data = nullptr;
	};

	class Texture {
	private:
		GLuint m_Texture;
		TextureType m_Type;
	public:
		Texture(TextureType);
		virtual ~Texture();
		
		void SetUnit(GLuint);

		void SetTextureInfo(TextureParamName, TextureParamValue);
		void LoadTexture(std::string_view, TextureInfo);

		GLuint GetID();
	};
}