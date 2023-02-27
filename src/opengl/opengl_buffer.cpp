#include "opengl_buffer.h"

#include <glad/glad.h>

namespace Kredo
{

uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::ShaderTypeFloat:
        return 4;

    case ShaderDataType::ShaderTypeFloat2:
        return 4 * 2;

    case ShaderDataType::ShaderTypeFloat3:
        return 4 * 3;

    case ShaderDataType::ShaderTypeFloat4:
        return 4 * 4;

    case ShaderDataType::ShaderTypeMat3:
        return 4 * 3 * 3;

    case ShaderDataType::ShaderTypeMat4:
        return 4 * 4 * 4;

    case ShaderDataType::ShaderTypeInt:
        return 4;

    case ShaderDataType::ShaderTypeInt2:
        return 4 * 2;

    case ShaderDataType::ShaderTypeInt3:
        return 4 * 3;

    case ShaderDataType::ShaderTypeInt4:
        return 4 * 4;

    case ShaderDataType::ShaderTypeBool:
        return 1;

    default:
        return 0;
    }

    return 0;
}

BufferElement::BufferElement(ShaderDataType type, const wxString& name, bool normalized)
    : type(type)
    , name(name)
    , size(ShaderDataTypeSize(type))
    , offset(0)
    , normalized(normalized)
{
}

uint32_t BufferElement::GetComponentCount() const
{
    switch (type)
    {
    case ShaderDataType::ShaderTypeFloat:
        return 1;

    case ShaderDataType::ShaderTypeFloat2:
        return 2;

    case ShaderDataType::ShaderTypeFloat3:
        return 3;

    case ShaderDataType::ShaderTypeFloat4:
        return 4;

    case ShaderDataType::ShaderTypeMat3:
        return 3;

    case ShaderDataType::ShaderTypeMat4:
        return 4;

    case ShaderDataType::ShaderTypeInt:
        return 1;

    case ShaderDataType::ShaderTypeInt2:
        return 2;

    case ShaderDataType::ShaderTypeInt3:
        return 3;

    case ShaderDataType::ShaderTypeInt4:
        return 4;

    case ShaderDataType::ShaderTypeBool:
        return 1;

    default:
        return 0;
    }

    return 0;
}


BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : _elements(elements)
{
    CalculateOffsetsAndStride();
}

uint32_t BufferLayout::GetStride() const
{
    return _stride;
}

const BufferElementVector& BufferLayout::GetElements() const
{
    return _elements;
}

BufferElementVector::iterator BufferLayout::begin()
{
    return _elements.begin();
}

BufferElementVector::iterator BufferLayout::end()
{
    return _elements.end();
}

BufferElementVector::const_iterator BufferLayout::begin() const
{
    return _elements.begin();
}

BufferElementVector::const_iterator BufferLayout::end() const
{
    return _elements.end();
}

void BufferLayout::CalculateOffsetsAndStride()
{
    _stride = 0;
    for (auto& element: _elements)
    {
        element.offset = _stride;
        _stride += element.size;
    }
}


OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
    glCreateBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void OpenGLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void OpenGLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& OpenGLVertexBuffer::GetLayout() const
{
    return _layout;
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
{
    _layout = layout;
}


OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
{
    glCreateBuffers(1, &_id);

    // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
    // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void OpenGLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void OpenGLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::GetCount() const
{
    return _count;
}

}
