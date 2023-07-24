#include "OGLCubeMap.h"

#include "glew.h"

OGLCubeMap::OGLCubeMap(std::filesystem::path path, CubeMapLayout layout)
{
	glGenTextures(1, &m_CubeMap);
	Bind();

	Image image(path.string(), ImageFormat::RGB, false);
	LoadCubeMapFaceLayout(image, layout);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void OGLCubeMap::SetFaceImage(CubeMapFace face, Image& image)
{
	//m_FaceMap[face] = image;
}

void OGLCubeMap::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
}

void OGLCubeMap::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

static std::vector<unsigned char> LoadFaceFromPos(Image image, int x, int y, int w, int h, CubeMapLayout layout)
{

	int cellSize = w;
	int startPosX = x;
	int startPosY = y;
	int width = w;  // Width of the region
	int height = h;  // Height of the region
	int image_channels = 3;

	std::vector<unsigned char> pixels((width * height) * image_channels);
	pixels.resize((width * height) * image_channels);

	unsigned char* image_data = image.GetRawData();
	int image_width = image.GetProperties().Width;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char* pixel = &image_data[((startPosY + y) * image_width + (startPosX + x)) * image_channels];


			for (int ch = 0; ch < image_channels; ch++)
			{
				//pixels[((y * width + x) * image_channels) + ch] = pixel[ch];

				pixels[(y * width + x) * image_channels + ch] = pixel[ch];
			}

		}
	}
	
	if (pixels.size())
		return pixels;
	else
		return std::vector<unsigned char>();
}

static int CubeMapFaceToOGL(CubeMapFace face)
{
	switch (face)
	{
		case CubeMapFace::Left:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case CubeMapFace::Right:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case CubeMapFace::Top:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case CubeMapFace::Bottom:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case CubeMapFace::Front:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case CubeMapFace::Back:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}
	return 0;
}

void OGLCubeMap::LoadCubeMapFaceLayout(Image& image, CubeMapLayout layout)
{
	if (layout == CubeMapLayout::HorizontalCross)
	{
		int cellSize = image.GetProperties().Width / 4;
		int width = cellSize;  // Width of the region
		int height = cellSize;  // Height of the region

		// left, right, front, back
		for (int i = 0; i < 4; i++)
		{
			int startPosX = cellSize * i + 1;
			int startPosY = cellSize;

			m_FaceMap[(CubeMapFace)i] = LoadFaceFromPos(image, startPosX, startPosY, width, height, layout);
		}

		// top & bottom
		m_FaceMap[CubeMapFace::Top] = LoadFaceFromPos(image, cellSize, 0, width, height, layout);
		m_FaceMap[CubeMapFace::Bottom]= LoadFaceFromPos(image, cellSize, cellSize * 2, width, height, layout);

		std::map<CubeMapFace, std::vector<unsigned char>>::iterator it;
		for (it = m_FaceMap.begin(); it != m_FaceMap.end(); it++)
		{			
			glTexImage2D(CubeMapFaceToOGL(it->first), 0, GL_RGB, cellSize, cellSize, 0, GL_RGB, GL_UNSIGNED_BYTE, it->second.data());
		}
	}
}

