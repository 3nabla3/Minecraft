#pragma once

#include "Hazel/Renderer/RendererAPI.h"
#include <glad/glad.h>


namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual inline void SetDepthFuncLessThanOrEqualTo() override { glDepthFunc(GL_LEQUAL); }
		virtual inline void SetDepthFuncLessThan() override { glDepthFunc(GL_LESS); }

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
	};
}