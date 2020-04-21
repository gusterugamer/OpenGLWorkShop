#pragma once
#include "Resources.h"
#include "../Utilities/esUtil.h"
#include <memory>
class Texture {
private:
	
	std::shared_ptr<TextureResource> pTr;
	GLuint id;	
	
public:

	const GLint GetTextureId() const;	
	Texture(std::shared_ptr<TextureResource> pTr);
	~Texture();	
	void Bind() const;
	void Unbind() const;
};