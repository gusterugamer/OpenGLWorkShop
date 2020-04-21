#pragma once

class IndexBuffer {
protected:
	unsigned int m_RendererID;
	unsigned int count;
public:
	IndexBuffer() = default;
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void DeleteBuffer() const;

	unsigned int GetCount() const;
};