#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>

namespace Hazel {

	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 TexCoord;
		float TexIndex;
	};

	struct RendererData
	{
		constexpr static uint32_t s_MaxQuads = 100000; // 1M
		constexpr static uint32_t s_MaxVerticies = s_MaxQuads * 8;
		constexpr static uint32_t s_MaxIndices = s_MaxQuads * 36;

		constexpr static uint32_t s_MaxTextureSlots = 192; // TODO: RenderCaps

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<TextureCubeMap> WhiteTexture;
		Ref<TextureCubeMap> Skybox;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		glm::vec4 CubeVertexPositions[8];

		// keeping these in ram is useful for rendering static objects
		glm::mat4 ProjectionViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::vec3 CameraPosition = glm::vec3(1.0f);
		glm::vec3 CameraRotation = glm::vec3(1.0f);
		float FOV;

		std::array<Ref<Texture>, s_MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		Ref<Shader> TextureShader;
		Ref<Shader> SkyboxShader;

		Renderer::Statistics Stats;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();
		RenderCommand::Init();

		s_Data.CubeVertexArray = Hazel::VertexArray::Create();

		s_Data.CubeVertexBuffer = Hazel::VertexBuffer::Create(s_Data.s_MaxVerticies * sizeof(CubeVertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float4, "a_Color"},
			{Hazel::ShaderDataType::Float3, "a_Texcoord"},
			{Hazel::ShaderDataType::Float, "a_TexIndex"}
			});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		uint32_t indices[36] = {
			0, 1, 2, 2, 3, 0, // front
			4, 5, 6, 6, 7, 4, // rear
			2, 3, 6, 6, 7, 3, // top
			1, 0, 5, 5, 4, 0, // bottom
			0, 3, 4, 4, 7, 3, // left
			1, 2, 5, 5, 6, 2  // right
		};

		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.s_MaxVerticies];

		uint32_t* cubeIndices = new uint32_t[s_Data.s_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.s_MaxIndices; i += 36)
		{
			cubeIndices[i +  0] = offset + 0;
			cubeIndices[i +  1] = offset + 1;
			cubeIndices[i +  2] = offset + 2;
			cubeIndices[i +  3] = offset + 2;
			cubeIndices[i +  4] = offset + 3;
			cubeIndices[i +  5] = offset + 0;
							 
			cubeIndices[i +  6] = offset + 4;
			cubeIndices[i +  7] = offset + 5;
			cubeIndices[i +  8] = offset + 6;
			cubeIndices[i +  9] = offset + 6;
			cubeIndices[i + 10] = offset + 7;
			cubeIndices[i + 11] = offset + 4;

			cubeIndices[i + 12] = offset + 2;
			cubeIndices[i + 13] = offset + 3;
			cubeIndices[i + 14] = offset + 6;
			cubeIndices[i + 15] = offset + 6;
			cubeIndices[i + 16] = offset + 7;
			cubeIndices[i + 17] = offset + 3;

			cubeIndices[i + 18] = offset + 1;
			cubeIndices[i + 19] = offset + 0;
			cubeIndices[i + 20] = offset + 5;
			cubeIndices[i + 21] = offset + 5;
			cubeIndices[i + 22] = offset + 4;
			cubeIndices[i + 23] = offset + 0;

			cubeIndices[i + 24] = offset + 0;
			cubeIndices[i + 25] = offset + 3;
			cubeIndices[i + 26] = offset + 4;
			cubeIndices[i + 27] = offset + 4;
			cubeIndices[i + 28] = offset + 7;
			cubeIndices[i + 29] = offset + 3;

			cubeIndices[i + 30] = offset + 1;
			cubeIndices[i + 31] = offset + 2;
			cubeIndices[i + 32] = offset + 5;
			cubeIndices[i + 33] = offset + 5;
			cubeIndices[i + 34] = offset + 6;
			cubeIndices[i + 35] = offset + 2;

			offset += 8;
		}

		auto indexBuffer = Hazel::IndexBuffer::Create(cubeIndices, s_Data.s_MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(indexBuffer);
		delete[] cubeIndices;

		s_Data.WhiteTexture = TextureCubeMap::Create(1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, 4); 

		int32_t samplers[s_Data.s_MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.s_MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.SkyboxShader = Shader::Create("assets/shaders/Skybox.glsl");
		s_Data.TextureShader = Shader::Create("assets/shaders/Textured3D.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.s_MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.CubeVertexPositions[0] = { -1.0f, -1.0f, -1.0f, 1.0f}; // 0 front bottom left
		s_Data.CubeVertexPositions[1] = {  1.0f, -1.0f, -1.0f, 1.0f}; // 1 front bottom right
		s_Data.CubeVertexPositions[2] = {  1.0f,  1.0f, -1.0f, 1.0f}; // 2 front top    right
		s_Data.CubeVertexPositions[3] = { -1.0f,  1.0f, -1.0f, 1.0f}; // 3 front top    left
															 
		s_Data.CubeVertexPositions[4] = { -1.0f, -1.0f,  1.0f, 1.0f}; // 4 rear  bottom left
		s_Data.CubeVertexPositions[5] = {  1.0f, -1.0f,  1.0f, 1.0f}; // 5 rear  bottom right
		s_Data.CubeVertexPositions[6] = {  1.0f,  1.0f,  1.0f, 1.0f}; // 6 rear  top    right
		s_Data.CubeVertexPositions[7] = { -1.0f,  1.0f,  1.0f, 1.0f}; // 7 rear  top    left
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
		s_Data.CameraPosition = camera.GetPosition();
		s_Data.CameraRotation = camera.GetRotation();
		s_Data.FOV = glm::radians(camera.GetFOV());
		s_Data.ProjectionViewMatrix = camera.GetProjectionViewMatrix();
		s_Data.ProjectionMatrix = camera.GetProjectionMatrix();
		s_Data.ViewMatrix = camera.GetViewMatrix();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1; // white texture = 0
	}

	void Renderer::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase); 
		if (dataSize != 0)
		{
			s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

			s_Data.CubeVertexArray->Bind();
			Flush();
		}
	}

	void Renderer::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer::FlushAndReset()
	{
		EndScene();

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1; // white texture = 0
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
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.CubeCount++;
	}

	static bool InFOV(const glm::vec3& objPos)
	{
		float zDist = objPos.z - s_Data.CameraPosition.z;
		float xDist = objPos.x - s_Data.CameraPosition.x;
		float angle = std::atanf(xDist / zDist);

		if (zDist > 0) angle += 3.1415f;
		else if (angle < 0) angle += 3.1415f * 2;

		float offset = s_Data.CameraRotation.y;
		float bottomBound = -s_Data.FOV + offset;
		float topBound = s_Data.FOV + offset;

		if (bottomBound < 0 && angle > bottomBound + 3.1415f * 2)
			return true;

		if (topBound > 3.1415f * 2 && angle < topBound - 3.1415f * 2)
			return true;

		return -s_Data.FOV + + offset < angle && angle < s_Data.FOV + + offset;
	}

	void Renderer::DrawColoredCube(const glm::vec3& position, const glm::vec4& color, const glm::vec3& size)
	{
		if (InFOV(position))
		{
			const float textureIndex = 0;

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), size);

			for (uint32_t i = 0; i < 8; i++)
			{
				s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
				s_Data.CubeVertexBufferPtr->Color = color;
				s_Data.CubeVertexBufferPtr->TexCoord = { s_Data.CubeVertexPositions[i].x, s_Data.CubeVertexPositions[i].y, s_Data.CubeVertexPositions[i].z };
				s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
				s_Data.CubeVertexBufferPtr++;
			}

			s_Data.CubeIndexCount += 36;
			s_Data.Stats.CubeCount++;
		}
	}

	void Renderer::DrawTexturedCube(const glm::vec3& position, const Ref<TextureCubeMap>& texture, const glm::vec3& size, const glm::vec4& tintColor)
	{
		if (InFOV(position))
		{
			if (s_Data.CubeIndexCount >= s_Data.s_MaxIndices)
				FlushAndReset();

			float textureIndex = 0;

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0)
			{
				if (s_Data.TextureSlotIndex == s_Data.s_MaxTextureSlots)
					FlushAndReset();
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), size);

			for (uint32_t i = 0; i < 8; i++)
			{
				s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
				s_Data.CubeVertexBufferPtr->Color = tintColor;
				s_Data.CubeVertexBufferPtr->TexCoord = { s_Data.CubeVertexPositions[i].x, s_Data.CubeVertexPositions[i].y, s_Data.CubeVertexPositions[i].z };
				s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
				s_Data.CubeVertexBufferPtr++;
			}

			s_Data.CubeIndexCount += 36;
			s_Data.Stats.CubeCount++;
		}
	}

	void Renderer::DrawSkybox(const Ref<TextureCubeMap>& texture)
	{
		RenderCommand::SetDepthFuncLessThanOrEqualTo();

		auto& shader = s_Data.SkyboxShader;
		shader->Bind();
		shader->SetMat4("u_Projection", s_Data.ProjectionMatrix);
		shader->SetMat4("u_View", glm::mat4(glm::mat3(s_Data.ViewMatrix)));

		//float vertecies[] = {
		//	-1.0f, -1.0f, -1.0f, // 0 front bottom left
		//	 1.0f, -1.0f, -1.0f, // 1 front bottom right
		//	 1.0f,  1.0f, -1.0f, // 2 front top    right
		//	-1.0f,  1.0f, -1.0f, // 3 front top    left

		//	-1.0f, -1.0f,  1.0f, // 4 rear  bottom left
		//	 1.0f, -1.0f,  1.0f, // 5 rear  bottom right
		//	 1.0f,  1.0f,  1.0f, // 6 rear  top    right
		//	-1.0f,  1.0f,  1.0f  // 7 rear  top    left
		//};

		CubeVertex* vertecies = new CubeVertex[8];

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		for (uint32_t i = 0; i < 8; i++)
		{
			vertecies[i].Position = s_Data.CubeVertexPositions[i];
			vertecies[i].Color = color;						   
			vertecies[i].TexCoord = { 1.0f, 1.0f, 1.0f };	   
			vertecies[i].TexIndex = 0;		
			// the last three do not matter,
			// they are there because I am using the
			// same vertex buffer as the other cubes
		}

		s_Data.CubeVertexBuffer->SetData(vertecies, 8 * sizeof(CubeVertex));

		s_Data.CubeVertexArray->Bind();
		texture->Bind(0);

		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, 36);
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.CubeCount++;
		RenderCommand::SetDepthFuncLessThan();
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::ResetStats()
	{
		s_Data.Stats.DrawCalls = 0;
		s_Data.Stats.CubeCount = 0;
	}

}