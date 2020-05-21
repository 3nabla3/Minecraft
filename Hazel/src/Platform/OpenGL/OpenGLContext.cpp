#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <Glad/Glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_WindowHandle, "Failed to create window handle!");
	}

	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialized Glad!");
		HZ_CORE_INFO("OpenGL info:");
		HZ_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
		//HZ_CORE_INFO("   Total texture units: {0}", glGetString(GL_MAX_TEXTURE_UNITS));
		int units;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &units);

	#ifdef HZ_ENABLE_ASSERTS
		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		HZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at leat OpenGL version 4.5!");
	#endif // HZ_ENABLE_ASSERTS
	}

	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}

}