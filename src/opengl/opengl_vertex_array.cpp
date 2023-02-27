#include "opengl_vertex_array.h"

#include <wx/log.h>
#include <glad/glad.h>

namespace Kredo
{

GLenum ToOpenGLType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;

    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;

    case ShaderDataType::Bool:
        return GL_BOOL;

    default:
        return 0;
    }

    return 0;
}


OpenGLVertexArray::OpenGLVertexArray()
    : _vertexBufferIndex(0)
{
    glCreateVertexArrays(1, &_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &_id);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(_id);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Shared<OpenGLVertexBuffer>& vertexBuffer)
{
    if (vertexBuffer->GetLayout().GetElements().size() == 0)
    {
        wxLogWarning("OpenGLVertexArray: vertex buffer has no layout");
        return;
    }

    glBindVertexArray(_id);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element: layout)
    {
        switch (element.type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        {
            glEnableVertexAttribArray(_vertexBufferIndex);
            glVertexAttribPointer(_vertexBufferIndex,
                                  element.GetComponentCount(),
                                  ToOpenGLType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  (const void*)element.offset);
            _vertexBufferIndex++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool:
        {
            glVertexAttribIPointer(_vertexBufferIndex,
                                   element.GetComponentCount(),
                                   ToOpenGLType(element.type),
                                   layout.GetStride(),
                                   (const void*)element.offset);
            _vertexBufferIndex++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
        {
            const auto count = element.GetComponentCount();
            for (auto i = 0; i < count; i++)
            {
                glEnableVertexAttribArray(_vertexBufferIndex);
                glVertexAttribPointer(_vertexBufferIndex,
                                      count,
                                      ToOpenGLType(element.type),
                                      element.normalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(_vertexBufferIndex, 1);
                _vertexBufferIndex++;
            }

            break;
        }

        default:
            wxLogWarning("OpenGLVertexArray: unknown shader data type");
        }
    }

    _vertexBuffers.push_back(vertexBuffer);
}

const std::vector<Shared<OpenGLVertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
{
    return _vertexBuffers;
}

void OpenGLVertexArray::SetIndexBuffer(const Shared<OpenGLIndexBuffer>& indexBuffer)
{
    glBindVertexArray(_id);
    indexBuffer->Bind();
    _indexBuffer = indexBuffer;
}

const Shared<OpenGLIndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
{
    return _indexBuffer;
}

}
