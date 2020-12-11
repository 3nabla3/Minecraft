#include "Terrain.h"
#include "PerlinNoise.hpp"
#include "Chunk.h"
#include <cstdlib>

Terrain::Terrain(int maxX, int maxZ, Hazel::Ref<Hazel::TextureCubeMap> texture)
	:m_MaxX(maxX), m_MaxZ(maxZ), m_Texture(texture)
{
	srand((unsigned int)time(NULL));
	m_Seed = rand();
	HZ_WARN("Seed is: {0}", m_Seed);
}

void Terrain::Render()
{
	siv::BasicPerlinNoise<float> gen(m_Seed);
	while (m_RunningZ < m_MaxZ)
	{
		while (m_RunningX < m_MaxX)
		{
			float arg1 = m_RunningX / 100.0f;
			float arg2 = m_RunningZ / 100.0f;
			int randBlockSize = (int)(gen.noise2D_0_1(arg1, arg2) * 10);
			//HZ_TRACE("Rand size: {0};\tArg: {1}", randBlockSize, arg);
			Chunk({ m_ChunkWidth, randBlockSize, m_ChunkLength }, { m_RunningX, 0, m_RunningZ }, m_Texture).Display();

			m_RunningX += m_ChunkWidth * 2;
		}
		m_RunningZ += m_ChunkLength * 2;
		m_RunningX = 0;
	}
	m_RunningX = 0;
	m_RunningZ = 0;
}