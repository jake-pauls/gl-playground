#include "VertexArray.hpp"

#include "renderer/Renderer.hpp"

VertexArray::VertexArray()
{
	GLExec(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLExec(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLExec(glEnableVertexAttribArray(i));
		GLExec(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLExec(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLExec(glBindVertexArray(0));
}