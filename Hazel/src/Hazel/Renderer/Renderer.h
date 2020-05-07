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

		static inline void SetSkybox(const Ref<TextureCubeMap>& skybox) { s_SceneData->Skybox = skybox; }
		static inline const Ref<TextureCubeMap>& GetSkybox() { return s_SceneData->Skybox; }
		
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f));

		static void DrawColoredCube(const glm::vec3& position, const glm::vec4& color, const glm::vec3& size);
		static void DrawTexturedCube(const glm::vec3& position, const Ref<TextureCubeMap>& texture, const glm::vec3& size);

		static void DrawSkybox(const Ref<TextureCubeMap>& texture);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ProjectionViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
			Ref<TextureCubeMap> Skybox;
		};

		static SceneData* s_SceneData;
		static Ref<VertexArray> s_VertexArray;
		static ShaderLibrary s_ShaderLibrary;
	};

}