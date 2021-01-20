#pragma once
#include <Hazel.h>

class Terrain
{
public:
	Terrain() = default;
	Terrain(int maxX, int maxZ, Hazel::Ref<Hazel::TextureCubeMap> texture);
	void Render(float maxHeight, float freq);
private:
	uint32_t m_Seed;

	// in coordinates not cube count
	int m_RunningX = 0;
	int m_RunningZ = 0;
	int m_MaxX;
	int m_MaxZ;

	// in cube count not coordinates
	int m_ChunkWidth = 15;
	int m_ChunkLength = 15;

	Hazel::Ref<Hazel::TextureCubeMap> m_Texture;
};
