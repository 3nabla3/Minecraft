#include "hzpch.h"
#include "RendererAPI.h"

namespace Hazel {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	void RendererAPI::SetClearColor(const glm::vec4& color)
	{
	}

	void RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
	}
	
}