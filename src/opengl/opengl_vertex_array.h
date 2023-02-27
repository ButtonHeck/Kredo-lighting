#pragma once

#include "opengl_buffer.h"

#include <vector>
#include <cstdint>
#include <memory>

namespace Kredo
{

class OpenGLVertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const std::shared_ptr<OpenGLVertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<OpenGLIndexBuffer> &indexBuffer);

    const std::vector<std::shared_ptr<OpenGLVertexBuffer>>& GetVertexBuffers() const;
    const std::shared_ptr<OpenGLIndexBuffer>& GetIndexBuffer() const;

private:
    uint32_t _id;
    uint32_t _vertexBufferIndex = 0;
    std::vector<std::shared_ptr<OpenGLVertexBuffer>> _vertexBuffers;
    std::shared_ptr<OpenGLIndexBuffer> _indexBuffer;
};

}
