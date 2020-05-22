#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {

	RendererAPI* const RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}