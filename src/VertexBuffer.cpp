#include "VertexBuffer.hpp"
#include "renderer/Renderer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{
    GLExec(glGenBuffers(1, &m_RendererID));
    GLExec(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLExec(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() 
{
    GLExec(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const 
{
    GLExec(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const 
{
    GLExec(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
