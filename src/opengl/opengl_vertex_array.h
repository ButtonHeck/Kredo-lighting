#pragma once

#include "pointers.h"
#include "opengl_buffer.h"

#include <vector>
#include <cstdint>

namespace Kredo
{

class OpenGLVertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const Shared<OpenGLVertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Shared<OpenGLIndexBuffer>& indexBuffer);

    const std::vector<Shared<OpenGLVertexBuffer>>& GetVertexBuffers() const;
    const Shared<OpenGLIndexBuffer>& GetIndexBuffer() const;

private:
    uint32_t _id;
    uint32_t _vertexBufferIndex = 0;
    std::vector<Shared<OpenGLVertexBuffer>> _vertexBuffers;
    Shared<OpenGLIndexBuffer> _indexBuffer;
};

}
