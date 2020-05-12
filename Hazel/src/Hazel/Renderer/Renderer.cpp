#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel {

	struct RendererData
	{
		constexpr static uint32_t s_MaxQuads = 100;
		constexpr static uint32_t s_MaxVerticies = s_MaxQuads * 8;
		constexpr static uint32_t s_MaxIndices = s_MaxQuads * 36;
		constexpr static uint32_t s_MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<TextureCubeMap> WhiteTexture;
		Ref<TextureCubeMap> Skybox;

		// keeping these in ram is useful for rendering static objects
		glm::mat4 ProjectionViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);

		ShaderLibrary ShaderLibrary;

		Renderer::Statistics stats;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();
		RenderCommand::Init();

		s_Data.CubeVertexArray = Hazel::VertexArray::Create();

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
		s_Data.CubeVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[36] = {
			0, 1, 2, 2, 3, 0, // front
			4, 5, 6, 6, 7, 4, // rear
			2, 3, 6, 6, 7, 3, // top
			1, 0, 5, 5, 4, 0, // bottom
			0, 3, 4, 4, 7, 3, // left
			1, 2, 5, 5, 6, 2  // right
		};

		auto indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data.CubeVertexArray->SetIndexBuffer(indexBuffer);

		s_Data.ShaderLibrary.Load("assets/shaders/Textured3D.glsl");
		s_Data.ShaderLibrary.Load("assets/shaders/Skybox.glsl");

		s_Data.WhiteTexture = TextureCubeMap::Create(1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, 4);
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data.ProjectionViewMatrix = camera.GetProjectionViewMatrix();
		s_Data.ProjectionMatrix = camera.GetProjectionMatrix();
		s_Data.ViewMatrix = camera.GetViewMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SetSkybox(const Ref<TextureCubeMap>& skybox)
	{
		s_Data.Skybox = skybox;
	}

	const Ref<TextureCubeMap>& Renderer::GetSkybox()
	{
		return s_Data.Skybox;
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", s_Data.ProjectionViewMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		s_Data.stats.DrawCalls++;
		s_Data.stats.CubeCount++;
	}

	void Renderer::DrawColoredCube(const glm::vec3& position, const glm::vec4& color, const glm::vec3& size)
	{
		auto shader = s_Data.ShaderLibrary.Get("Textured3D");

		shader->Bind();
		shader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture->Bind();

		auto tranform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		Submit(shader, s_Data.CubeVertexArray, tranform);
	}

	void Renderer::DrawTexturedCube(const glm::vec3& position, const Ref<TextureCubeMap>& texture, const glm::vec3& size)
	{
		auto shader = s_Data.ShaderLibrary.Get("Textured3D");

		shader->Bind();
		shader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		Submit(shader, s_Data.CubeVertexArray, transform);
	}

	void Renderer::DrawSkybox(const Ref<TextureCubeMap>& texture)
	{
		RenderCommand::SetDepthFuncLessThanOrEqualTo();
		auto shader = s_Data.ShaderLibrary.Get("Skybox");
		shader->Bind();
		shader->SetMat4("u_Projection", s_Data.ProjectionMatrix);
		shader->SetMat4("u_View", glm::mat4(glm::mat3(s_Data.ViewMatrix)));
		s_Data.CubeVertexArray->Bind();
		texture->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		s_Data.stats.DrawCalls++;
		s_Data.stats.CubeCount++;
		RenderCommand::SetDepthFuncLessThan();
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.stats;
	}

	void Renderer::ResetStats()
	{
		s_Data.stats.DrawCalls = 0;
		s_Data.stats.CubeCount = 0;
	}

}