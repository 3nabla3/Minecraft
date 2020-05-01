#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include <Hazel\Renderer\Texture.h>

namespace Hazel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, const glm::vec2& size = { 1.0f, 1.0f }, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 0.0f }, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, const glm::vec2& size = { 1.0f, 1.0f }, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 0.0f }, float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const Ref<Texture>& texture, const glm::vec2& size = { 1.0f, 1.0f }, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 0.0f }, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const Ref<Texture>& texture, const glm::vec2& size = { 1.0f, 1.0f }, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 0.0f }, float tilingFactor = 1.0f);


	private:
		static Ref<VertexArray> s_QuadVertexArray;
		static Ref<Shader> s_TextureShader;
		static Ref<Texture2D> s_WhiteTexture;
	};

}