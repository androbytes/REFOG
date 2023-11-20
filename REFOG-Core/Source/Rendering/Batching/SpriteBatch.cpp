#include "REFOG/Rendering/Batching/SpriteBatch.h"
#include "REFOG/Rendering/Camera.h"

namespace REFOG {
	std::pair<std::vector<float>, std::vector<unsigned int>> SpriteBatch::ComputeIndicies(std::vector<float> vertices) {
		std::pair<std::vector<float>, std::vector<unsigned int>> Result = std::make_pair(std::vector<float>(), std::vector<unsigned int>());
		std::map<std::vector<float>, unsigned int> VertexIndex;
		std::vector<float> VertexBuffer;
		for (float vertex : vertices) {
			VertexBuffer.push_back(vertex);
			if (VertexBuffer.size() == m_Stride) {
				std::vector<float> verticesSubvector = { VertexBuffer.begin(), VertexBuffer.begin() + 3 };
				auto setIndex = VertexIndex.find(verticesSubvector);
				if (setIndex == VertexIndex.end()) {
					Result.second.push_back(Result.second.size());
					VertexIndex.emplace(std::make_pair(verticesSubvector, Result.second.size()));
					Result.first.reserve(Result.first.size() + 3);
					Result.first.insert(Result.first.end(), verticesSubvector.begin(), verticesSubvector.end());
				}
				else {
					Result.second.push_back(setIndex->second);
				}
				VertexBuffer.clear();
			}
		}
		return Result;
	}

	SpriteBatch::SpriteBatch(bool CreateBufferInRenderCall) {
		CBRC = CreateBufferInRenderCall;
		m_VA = new VertexArray();
		m_Shader = new Shader();
	}

	SpriteBatch::~SpriteBatch() {
		delete m_VA;
		delete m_Shader;
	}

	void SpriteBatch::InitObjects(std::vector<VertexElement> elements, std::vector<std::string_view> shaderPaths) {
		uint32_t elementCount = VertexArray::GetVertexElementsSize(elements) / sizeof(float);
		m_Stride = elementCount;

		m_VA->SetData(elements);

		int idx = 0;
		for (std::string_view path : shaderPaths) {
			m_Shader->LoadShader(path.data(), (ShaderType)idx);
			idx++;
		}
		m_Shader->LinkProgram();
	}

	SpriteBatchObject& SpriteBatch::AddObject(std::vector<float> Vertices, GLuint Texture) {
		m_TextureToVertices.insert({Texture, Vertices });

		SpriteBatchObject* obj = new SpriteBatchObject{ *this, m_BatchObjects.size() };
		m_BatchObjects.push_back(obj);
		return *obj;
	}

	void SpriteBatch::RemoveObject(int index) {
		m_BatchObjects[index] = nullptr;
	}

	void SpriteBatch::CreateBuffers() {
		for (auto pair : m_TextureToVertices) {
			auto data = ComputeIndicies(pair.second);
			Buffer* vBuffer = new Buffer();
			vBuffer->SetData(data.first.data(), data.first.size() * sizeof(float));
			Buffer* iBuffer = new Buffer();
			iBuffer->SetData(data.second.data(), data.second.size() * sizeof(float));
			m_TextureToBuffers.insert({ pair.first, std::make_pair(vBuffer, iBuffer) });
		}
	}

	void SpriteBatch::Render() {
		m_Shader->SetMat4("ViewProjection", REFOG::GlobalCamera->GetProjection() * REFOG::GlobalCamera->GetView());
		m_Shader->SetMat4("Model", glm::mat4(1.0f));
		m_Shader->Use();

		for (auto pair : m_TextureToBuffers) {
			m_VA->AddVertexBuffer(*pair.second.first);
			m_VA->AddElementBuffer(*pair.second.second);
			m_VA->Bind();
			glDrawElements(GL_TRIANGLES, pair.second.second->GetDataSize() / sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
		}
		m_VA->Unbind();
	}
}