#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(45.0f, 1280.0f / 720.0f)
	{
		/////// Triangle ///////////////////////////////////////////////////////////////
		m_VertexArray = Hazel::VertexArray::Create();

		float triangleVertecies[3 * 7] = { // create the vertecies on the stack
			-0.5f, -0.5f, 0.0,		1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0,		0.0f, 0.0f, 1.0f, 1.0f
		};

		// uploading triangle vertecies to vram
		Hazel::Ref<Hazel::VertexBuffer> vb;
		vb = Hazel::VertexBuffer::Create(triangleVertecies, sizeof(triangleVertecies));
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vb->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vb);

		// uploading triangle indicies to vram
		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> ib;
		ib = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(ib);


		/////// Square tiles ///////////////////////////////////////////////////////////
		m_SquareVA = Hazel::VertexArray::Create();

		float squareVertecies[4 * 5] = { // create the vertecies on the stack
			-0.5f, -0.5f, 0.0,		0.0f, 0.0f,	// bottom left
			 0.5f, -0.5f, 0.0,		1.0f, 0.0f,	// bottom right
			 0.5f,  0.5f, 0.0,		1.0f, 1.0f,	// top right
			-0.5f,  0.5f, 0.0,		0.0f, 1.0f	// top left
		};
		
		// uploading square vertecies to vram
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB = Hazel::VertexBuffer::Create(squareVertecies, sizeof(squareVertecies));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		// uploading square indices to vram
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 }; // create the indices on the stack
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);
		
		/////// Load shaders ///////////////////////////////////////////////////////////
		m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");
		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0); // 0 is the slot in which we bind the texture

		/////// Load textures //////////////////////////////////////////////////////////
		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = Hazel::Texture2D::Create("assets/textures/DonkeyKong.png");
	}

	void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	void OnUpdate(Hazel::TimeStep ts) override
	{
		m_CameraController.OnUpdate(ts);
		Hazel::RenderCommand::Clear();

		///// Render Scene ////////////////////////////////////////////////////
		{
			Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind(0);
			Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f * m_LogoTexture->GetHeight() / m_LogoTexture->GetWidth(), 1.0f)));
			m_LogoTexture->Bind(0);
			Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));


			m_ShaderLibrary.Get("FlatColor")->Bind();

			for (int x = 0; x < 15; x++)
			{
				for (int y = 0; y < 15; y++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 tranform = glm::translate(glm::mat4(1.0f), pos) * scale;

					m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", m_SquareColor);

					Hazel::Renderer::Submit(m_ShaderLibrary.Get("FlatColor"), m_SquareVA, tranform);
				}
			}

			Hazel::Renderer::EndScene();
		}
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Change Color!");

		ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_LogoTexture;

	Hazel::PerspectiveCameraController m_CameraController;
	glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.3f, 0.2f, 0.5f);
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		GetWindow().SetVSync(false);
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());

		Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f });
	}

	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}