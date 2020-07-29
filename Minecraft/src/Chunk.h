#pragma once
#include <Hazel.h>

// n * n * n blocks
class Chunk
{
public:
	Chunk(glm::vec3 size, const glm::vec3& position, Hazel::Ref<Hazel::TextureCubeMap> texture)
		:m_Size(size), m_Position(position), m_Texture(texture)
	{
	}

	void Display() 
	{
#ifdef HZ_DEBUG
		Intr_DisplayDebug();
#else
		Intr_DisplayOptimized();
#endif
	}

private:
	void Intr_DisplayDebug()
	{
		glm::vec3 bounds = m_Size * glm::vec3(2);
		for (uint32_t i = 0; i < bounds.x; i += 2)
			for (uint32_t j = 0; j < bounds.y; j += 2)
				for (uint32_t k = 0; k < bounds.z; k += 2)
						Hazel::Renderer::DrawTexturedCube({ m_Position.x + i,  m_Position.y + j,  m_Position.z + k }, m_Texture, { 1, 1, 1 });
	}

	void Intr_DisplayOptimized()
	{
		glm::vec3 bounds = m_Size * glm::vec3(2);
		for (uint32_t i = 0; i < bounds.x; i += 2)
			for (uint32_t j = 0; j < bounds.y; j += 2)
				for (uint32_t k = 0; k < bounds.z; k += 2)
					if (i == 0 || i == bounds.x - 2 || j == 0 || j == bounds.y - 2 || k == 0 || k == bounds.z - 2)
						Hazel::Renderer::DrawTexturedCube({ m_Position.x + i,  m_Position.y + j,  m_Position.z + k }, m_Texture, { 1, 1, 1 });
	}

	glm::vec3 m_Size;
	glm::vec3 m_Position;
	Hazel::Ref<Hazel::TextureCubeMap> m_Texture;
};