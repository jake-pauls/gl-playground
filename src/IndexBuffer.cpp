#include "IndexBuffer.hpp"
#include "renderer/Renderer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
  : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLExec(glGenBuffers(1, &m_RendererID));
    GLExec(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLExec(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() 
{
    GLExec(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const 
{
    GLExec(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const 
{
    GLExec(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
