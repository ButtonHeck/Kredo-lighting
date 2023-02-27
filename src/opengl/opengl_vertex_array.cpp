#include "opengl_vertex_array.h"

#include <wx/log.h>
#include <glad/glad.h>

namespace Kredo
{

GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return GL_FLOAT;

    case ShaderDataType::Float2:
        return GL_FLOAT;

    case ShaderDataType::Float3:
        return GL_FLOAT;

    case ShaderDataType::Float4:
        return GL_FLOAT;

    case ShaderDataType::Mat3:
        return GL_FLOAT;

    case ShaderDataType::Mat4:
        return GL_FLOAT;

    case ShaderDataType::Int:
        return GL_INT;

    case ShaderDataType::Int2:
        return GL_INT;

    case ShaderDataType::Int3:
        return GL_INT;

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

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<OpenGLVertexBuffer>& vertexBuffer)
{
    if (vertexBuffer->GetLayout().GetElements().size() == 0)
    {
        wxLogWarning("Vertex buffer has no layout");
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
                                  ShaderDataTypeToOpenGLBaseType(element.type),
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
                                   ShaderDataTypeToOpenGLBaseType(element.type),
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
                                      ShaderDataTypeToOpenGLBaseType(element.type),
                                      element.normalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(_vertexBufferIndex, 1);
                _vertexBufferIndex++;
            }

            break;
        }

        default:
            wxLogWarning("Unknown shader data type");
        }
    }

    _vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<OpenGLIndexBuffer>& indexBuffer)
{
    glBindVertexArray(_id);
    indexBuffer->Bind();
    _indexBuffer = indexBuffer;
}

const std::vector<std::shared_ptr<OpenGLVertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
{
    return _vertexBuffers;
}

const std::shared_ptr<OpenGLIndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
{
    return _indexBuffer;
}

}
