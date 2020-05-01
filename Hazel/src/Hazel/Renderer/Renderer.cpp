#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel {

	Ref<VertexArray> Renderer::s_VertexArray;
	ShaderLibrary Renderer::s_ShaderLibrary;

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();

		s_VertexArray = Hazel::VertexArray::Create();

		float vertecies[] = {
			-1.0f, -1.0f, -1.0f, // 0 front bottom left
			 1.0f, -1.0f, -1.0f, // 1 front bottom right
			 1.0f,  1.0f, -1.0f, // 2 front top    right
			-1.0f,  1.0f, -1.0f, // 3 front top    left
						   
			-1.0f, -1.0f,  1.0f, // 4 rear  bottom left
			 1.0f, -1.0f,  1.0f, // 5 rear  bottom right
			 1.0f,  1.0f,  1.0f, // 6 rear  top    right
			-1.0f,  1.0f,  1.0f  // 7 rear  top    left
		};

		auto vertexBuffer = Hazel::VertexBuffer::Create(vertecies, sizeof(vertecies));
		vertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"}
			});
		s_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[36] = {
			0, 1, 2, 2, 3, 0, // front
			4, 5, 6, 6, 7, 4, // rear
			2, 3, 6, 6, 7, 3, // top
			1, 0, 5, 5, 4, 0, // bottom
			0, 3, 4, 4, 7, 3, // left
			1, 2, 5, 5, 6, 2  // right
		};

		auto indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_VertexArray->SetIndexBuffer(indexBuffer);
		s_ShaderLibrary.Load("assets/shaders/VertexPos.glsl");
		s_ShaderLibrary.Load("assets/shaders/Textured3D.glsl");
		s_ShaderLibrary.Load("assets/shaders/Skybox.glsl");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const PerspectiveCamera& camera)
	{
		s_SceneData->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
		s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
		s_SceneData->ViewMatrix = camera.GetViewMatrix();
	}

	void Renderer::EndScene()
	{
		DrawSkybox(s_SceneData->Skybox);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", s_SceneData->ProjectionViewMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::DrawColoredCube(const glm::vec3& position, const glm::vec4& color, const glm::vec3& size)
	{
		auto shader = s_ShaderLibrary.Get("VertexPos");

		shader->Bind();
		shader->SetFloat4("u_Color", color);

		// T * S
		auto tranform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		Submit(shader, s_VertexArray, tranform);
	}

	void Renderer::DrawTexturedCube(const glm::vec3& position, const Ref<TextureCubeMap>& texture, const glm::vec3& size)
	{
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		Submit(s_ShaderLibrary.Get("Textured3D"), s_VertexArray, transform);
	}

	void Renderer::DrawSkybox(const Ref<TextureCubeMap>& texture)
	{
		RenderCommand::SetDepthFuncLessThanOrEqualTo();
		auto shader = s_ShaderLibrary.Get("Skybox");
		shader->Bind();
		shader->SetMat4("u_Projection", s_SceneData->ProjectionMatrix);
		shader->SetMat4("u_View", glm::mat4(glm::mat3(s_SceneData->ViewMatrix)));
		s_VertexArray->Bind();
		texture->Bind();
		RenderCommand::DrawIndexed(s_VertexArray);
		RenderCommand::SetDepthFuncLessThan();
	}

}