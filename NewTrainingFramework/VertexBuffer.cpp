#include "stdafx.h"
#include "VertexBuffer.h"
#include "DebugModeFunctions.h"
#include "Vertex.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
	:
	m_RendererID(0)
{
	GLCall(glGenBuffers(1, &m_RendererID));	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), data, GL_DYNAMIC_DRAW));	
}

VertexBuffer::~VertexBuffer()
{	
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::DeleteBuffer() const
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::CreateDynamicBuffer(const int & size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));
}
