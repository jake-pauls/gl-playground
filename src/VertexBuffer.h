#ifndef VertexBuffer_h
#define VertexBuffer_h

class VertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};

#endif /* VertexBuffer_h */
