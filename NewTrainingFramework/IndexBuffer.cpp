#include "stdafx.h"
#include "IndexBuffer.h"
#include "../Utilities/esUtil.h"
#include "OpenGLErrorChecking.h"

IndexBuffer::IndexBuffer(const void * data, unsigned int count)
	:m_RendererID(0), count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(GLushort), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{	
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::DeleteBuffer() const
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

unsigned int IndexBuffer::GetCount() const {return count;}

