#pragma once

#include <wx/string.h>
#include <vector>
#include <cstdint>

namespace Kredo
{

enum class ShaderDataType
{
    ShaderTypeNone = 0,
    ShaderTypeFloat,
    ShaderTypeFloat2,
    ShaderTypeFloat3,
    ShaderTypeFloat4,
    ShaderTypeMat3,
    ShaderTypeMat4,
    ShaderTypeInt,
    ShaderTypeInt2,
    ShaderTypeInt3,
    ShaderTypeInt4,
    ShaderTypeBool
};

uint32_t ShaderDataTypeSize(ShaderDataType type);

struct BufferElement
{
    BufferElement() = default;
    BufferElement(ShaderDataType type, const wxString& name, bool normalized = false);

    uint32_t GetComponentCount() const;

    ShaderDataType type;
    wxString name;
    uint32_t size;
    size_t offset;
    bool normalized;
};

using BufferElementVector = std::vector<BufferElement>;


class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    uint32_t GetStride() const;
    const BufferElementVector& GetElements() const;

    BufferElementVector::iterator begin();
    BufferElementVector::iterator end();
    BufferElementVector::const_iterator begin() const;
    BufferElementVector::const_iterator end() const;

private:
    void CalculateOffsetsAndStride();

private:
    BufferElementVector _elements;
    uint32_t _stride = 0;
};


class OpenGLVertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    ~OpenGLVertexBuffer();

    void Bind() const;
    void Unbind() const;
    void SetData(const void* data, uint32_t size);

    const BufferLayout& GetLayout() const;
    void SetLayout(const BufferLayout& layout);

private:
    uint32_t _id;
    BufferLayout _layout;
};


class OpenGLIndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t GetCount() const;

private:
    uint32_t _id;
    uint32_t _count;
};

}
