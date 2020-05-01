#include "hzpch.h"
#include "Renderer2D.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	Ref<VertexArray> Renderer2D::s_QuadVertexArray;
	Ref<Shader> Renderer2D::s_TextureShader;
	Ref<Texture2D> Renderer2D::s_WhiteTexture;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();
		s_QuadVertexArray = VertexArray::Create();

		float squareVertecies[4 * 5] = { // create the vertecies on the stack
			-0.5f, -0.5f, 0.0,		0.0f, 0.0f,	// bottom left
			 0.5f, -0.5f, 0.0,		1.0f, 0.0f,	// bottom right
			 0.5f,  0.5f, 0.0,		1.0f, 1.0f,	// top right
			-0.5f,  0.5f, 0.0,		0.0f, 1.0f,	// top left
		};

		// uploading square vertecies to vram
		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertecies, sizeof(squareVertecies));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		s_QuadVertexArray->AddVertexBuffer(squareVB);

		// uploading square indices to vram
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 }; // create the indices on the stack
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_QuadVertexArray->SetIndexBuffer(squareIB);

		s_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteTextureData, 4);

		// in a string to avoid forcing the client to have the shaders installed
		auto vSource = R"(
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ProjectionView;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ProjectionView * u_Transform * vec4(a_Position, 1.0);
}
)";

		auto fSource = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;
uniform vec4 u_Color;

void main()
{
	color = u_Color * texture(u_Texture, v_TexCoord * u_TilingFactor);
}
)";

		s_TextureShader = Shader::Create("Texture", vSource, fSource);
		s_TextureShader->Bind();
		s_TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();
		s_TextureShader->Bind();
		s_TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
		s_QuadVertexArray->Bind();
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec4& color, const glm::vec2& size)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, color, size);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color, const glm::vec2& size)
	{
		HZ_PROFILE_FUNCTION();
		s_WhiteTexture->Bind();

		s_TextureShader->SetFloat4("u_Color", color);
		s_TextureShader->SetFloat("u_TilingFactor", 1.0f);

		// Translation * Rotation * Scale
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_TextureShader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(s_QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const Ref<Texture>& texture, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, texture, size, tintColor, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const Ref<Texture>& texture, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();
		texture->Bind();

		s_TextureShader->SetFloat4("u_Color", tintColor);
		s_TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		// Translation * Rotation * Scale
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_TextureShader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(s_QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec4& color, const glm::vec2& size)
	{
		DrawQuad({ position.x, position.y, 0.0f }, color, size);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec4& color, const glm::vec2& size)
	{
		HZ_PROFILE_FUNCTION();
		s_WhiteTexture->Bind();

		s_TextureShader->SetFloat4("u_Color", color);
		s_TextureShader->SetFloat("u_TilingFactor", 1.0f);

		// Translation * Rotation * Scale
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_TextureShader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(s_QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.5f}, texture, size, tintColor, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();
		texture->Bind();

		s_TextureShader->SetFloat4("u_Color", tintColor);
		s_TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		// Translation * Rotation * Scale
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_TextureShader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(s_QuadVertexArray);
	}

}