#pragma once

class IndexBuffer {
protected:
	unsigned int m_RendererID;
	unsigned int count;
public:
	IndexBuffer() = default;
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void DeleteBuffer() const noexcept;

	unsigned int GetCount() const noexcept;
};