#include "opengl_vertex_array.h"

#include <wx/log.h>
#include <glad/glad.h>

namespace Kredo
{

GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::ShaderTypeFloat:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeFloat2:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeFloat3:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeFloat4:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeMat3:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeMat4:
        return GL_FLOAT;

    case ShaderDataType::ShaderTypeInt:
        return GL_INT;

    case ShaderDataType::ShaderTypeInt2:
        return GL_INT;

    case ShaderDataType::ShaderTypeInt3:
        return GL_INT;

    case ShaderDataType::ShaderTypeInt4:
        return GL_INT;

    case ShaderDataType::ShaderTypeBool:
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
        case ShaderDataType::ShaderTypeFloat:
        case ShaderDataType::ShaderTypeFloat2:
        case ShaderDataType::ShaderTypeFloat3:
        case ShaderDataType::ShaderTypeFloat4:
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
        case ShaderDataType::ShaderTypeInt:
        case ShaderDataType::ShaderTypeInt2:
        case ShaderDataType::ShaderTypeInt3:
        case ShaderDataType::ShaderTypeInt4:
        case ShaderDataType::ShaderTypeBool:
        {
            glVertexAttribIPointer(_vertexBufferIndex,
                                   element.GetComponentCount(),
                                   ShaderDataTypeToOpenGLBaseType(element.type),
                                   layout.GetStride(),
                                   (const void*)element.offset);
            _vertexBufferIndex++;
            break;
        }
        case ShaderDataType::ShaderTypeMat3:
        case ShaderDataType::ShaderTypeMat4:
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
