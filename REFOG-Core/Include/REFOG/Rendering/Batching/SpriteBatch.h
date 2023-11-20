#pragma once
#include "SpriteBatchObject.h"
#include "../Mesh.h"
#include <vector>
#include <map>

namespace REFOG {
	class SpriteBatch {
	private:
		std::map<GLuint, std::pair<Buffer*, Buffer*>> m_TextureToBuffers;
		std::map<GLuint, std::vector<float>> m_TextureToVertices; // temporary storage
		std::vector<BatchObject*> m_BatchObjects;
		VertexArray* m_VA;
		Shader* m_Shader;
		uint32_t m_Stride;

		// Create Buffers in Render Call
		bool CBRC;
		
		std::pair<std::vector<float>, std::vector<unsigned int>> ComputeIndicies(std::vector<float>);
	public:
		SpriteBatch(bool CreateBuffersInRenderCall);
		virtual ~SpriteBatch();

		void InitObjects(std::vector<VertexElement>, std::vector<std::string_view> shaderPaths);

		SpriteBatchObject& AddObject(std::vector<float> Vertices, GLuint Texture);
		void RemoveObject(int Index);
		void CreateBuffers();
		
		void Render();
	};
}