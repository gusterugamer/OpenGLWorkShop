#include "stdafx.h"
#include "Texture.h"
#include "../Utilities/TGA.h"
#include <iostream>
#include "DebugModeFunctions.h"
#include "ResourceManager.h"

const GLint Texture::GetTextureId() const
{
	return id;
}

Texture::Texture(std::shared_ptr<TextureResource> pTr)
	: pTr(pTr)
{

	// incarcare textura si crearea unui buffer pentru aceasta
	GLint t_width;
	GLint t_height;
	GLint t_bpp;
	
	GLchar* ap = LoadTGA(pTr->path.c_str(), &t_width, &t_height, &t_bpp);	
	GLCall(glGenTextures(1, &id));
	GLCall(glBindTexture(ResourceManager::GetInstance()->glStringToInt[pTr->type], id));

	GLCall(glTexParameteri(ResourceManager::GetInstance()->glStringToInt[pTr->type], GL_TEXTURE_MIN_FILTER, ResourceManager::GetInstance()->glStringToInt[pTr->min_filter]));
	GLCall(glTexParameteri(ResourceManager::GetInstance()->glStringToInt[pTr->type], GL_TEXTURE_MAG_FILTER, ResourceManager::GetInstance()->glStringToInt[pTr->mag_filter]));

	GLCall(glTexParameteri(ResourceManager::GetInstance()->glStringToInt[pTr->type], GL_TEXTURE_WRAP_S, ResourceManager::GetInstance()->glStringToInt[pTr->wrap_s]));
	GLCall(glTexParameteri(ResourceManager::GetInstance()->glStringToInt[pTr->type], GL_TEXTURE_WRAP_T, ResourceManager::GetInstance()->glStringToInt[pTr->wrap_t]));

	if (t_bpp == 24)
	{
		if (pTr->type == "2d")
		{
			GLCall(glTexImage2D(ResourceManager::GetInstance()->glStringToInt[pTr->type], 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ap));
			GLCall(glBindTexture(ResourceManager::GetInstance()->glStringToInt[pTr->type], 0));			
		}
		else if (pTr->type == "cube")
		{		
			std::string fata1;	

			for (int i = t_height / 3; i < 2*t_height/3; i++)
			{
				for (int j = 0; j < 3*t_width/4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width + 2*t_width/4) + j);
				}
			}			
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, t_width/4, t_height/3, 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));			
			fata1.clear();

			for (int i = t_height / 3; i < 2 * t_height / 3; i++)
			{
				for (int j = 0; j < 3 * t_width / 4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width) + j);
				}
			}
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, t_width/4 , t_height /3, 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));
			fata1.clear();	

			for (int i = 0; i < t_height / 3; i++)
			{
				for (int j = 0; j < 3 * t_width / 4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width + t_width / 4) + j);
				}
			}
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, t_width/4 , t_height/3 , 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));			
			fata1.clear();

			for (int i = 2 * t_height / 3; i < t_height; i++)
			{
				for (int j = 0; j < 3 * t_width / 4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width + t_width / 4) + j);
				}
			}


			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, t_width/4 , t_height/3 , 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));			
			fata1.clear();

			for (int i = t_height / 3; i < 2 * t_height / 3; i++)
			{
				for (int j = 0; j < 3 * t_width / 4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width + t_width / 4) + j);
				}
			}			
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, t_width/4 , t_height /3, 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));			
			fata1.clear();

			for (int i = t_height / 3; i < 2 * t_height / 3; i++)
			{
				for (int j = 0; j < 3 * t_width / 4; j++)
				{
					fata1 += *(ap + 3 * (i * t_width + 3 * t_width / 4) + j);
				}
			}
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, t_width/4 , t_height/3 , 0, GL_RGB, GL_UNSIGNED_BYTE, fata1.data()));			

			GLCall(glBindTexture(ResourceManager::GetInstance()->glStringToInt[pTr->type], 0));
		}

	}
	else if (t_bpp == 32)
	{
		if (pTr->type == "2d")
		{
			GLCall(glTexImage2D(ResourceManager::GetInstance()->glStringToInt[pTr->type], 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ap));
			GLCall(glBindTexture(ResourceManager::GetInstance()->glStringToInt[pTr->type], 0));
		}	
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind() const
{
	GLCall(glBindTexture(ResourceManager::GetInstance()->glStringToInt[pTr->type], id));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
