#include "REFOG/REFOG.h"
#include "glm/gtc/matrix_transform.hpp"

float vertices[] = {
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
};

float vertices2[] = {
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
};

GLuint indices[] = {
	0, 1, 2, 0, 2, 3
};

class OpenGLTest {
public:
	void Run() {
		REFOG::Window window = REFOG::Window("My Window", { 1280, 720 });
		REFOG::Mesh mesh = REFOG::Mesh();
		mesh.SetupShader({ REFOG::ShaderFile::Sprite });
		mesh.SetupVertexBuffer(vertices, sizeof(vertices));
		mesh.SetupIndexBuffer(indices, sizeof(indices));
		mesh.SetupVertexArray(REFOG::VertexShaderElements::SpriteShader);

		REFOG::Texture texture = REFOG::Texture(REFOG::Texture2D);
		texture.SetTextureInfo(REFOG::TextureWrapS, REFOG::Repeat);
		texture.SetTextureInfo(REFOG::TextureWrapT, REFOG::Repeat);
		texture.SetTextureInfo(REFOG::TextureMin, REFOG::Nearest);
		texture.SetTextureInfo(REFOG::TextureMag, REFOG::Linear);
		texture.LoadTexture("base.png", { REFOG::RGB });

		REFOG::GlobalCamera = std::make_unique<REFOG::Camera>();
		REFOG::GlobalCamera->SetPositionAndRotation({ 0, 0, 1.0f }, { 0, 0, 0 });

		{
			glm::vec2 viewportSize = window.GetWindowSize();
			REFOG::GlobalCamera->Ortho({ 0.0f, viewportSize.x, 0.0f, viewportSize.y });
		}

		mesh.GetShader()->SetMat4("ViewProjection", REFOG::GlobalCamera->GetProjection() * REFOG::GlobalCamera->GetView());
		mesh.GetShader()->SetMat4("Model", glm::scale(glm::mat4(1.0f), { 100, 100, 100 }));
		GLuint Extensions[16] = { 1 };
		mesh.GetShader()->SetUIntArray("Extensions", Extensions, 16);
		mesh.GetShader()->SetVec2("lightPos", { 0.5f, 0.5f });

		REFOG::Font Arial;
		Arial.LoadFont("C:\\Windows\\Fonts\\arial.ttf");
		Arial.LoadChars();

		REFOG::TextRenderer textRenderer(Arial);
		textRenderer.UpdateShader();

		while (window.IsOpen()) {
			window.SetupFramebuffer();
			glClear(GL_COLOR_BUFFER_BIT);

			mesh.Bind();
			texture.SetUnit(0);
			mesh.Render();
			mesh.Unbind();

			textRenderer.Render({
				"Hello World!",
				{25.0f, 25.0f},
				1.0f,
				{1.0f, 0.0f, 0.0f}
				});

			window.SwapBuffer();
		}
	}
};