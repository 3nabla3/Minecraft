#pragma once

#include "RenderCommand.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace Hazel {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void SetSkybox(const Ref<TextureCubeMap>& skybox);
		static const Ref<TextureCubeMap>& GetSkybox();
		
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f));

		static void DrawColoredCube(const glm::vec3& position, const glm::vec4& color, const glm::vec3& size);
		static void DrawTexturedCube(const glm::vec3& position, const Ref<TextureCubeMap>& texture, const glm::vec3& size);
		static void DrawWhiteCube(const glm::vec3& position);

		static void DrawSkybox(const Ref<TextureCubeMap>& texture);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t CubeCount = 0;

			uint32_t GetTotalVertexCount() { return CubeCount * 8; }
			uint32_t GetTotalIndexCount() { return CubeCount * 36; }
		};

		static void ResetStats();
		static Statistics GetStats();
	};

}