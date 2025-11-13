#pragma once

#include "Rendering/Buffer.h"

namespace Vest {

class VulkanVertexBuffer : public VertexBuffer {
public:
    explicit VulkanVertexBuffer(uint32_t /*size*/);
    VulkanVertexBuffer(float* /*vertices*/, uint32_t /*size*/);
    ~VulkanVertexBuffer() override = default;

    void Bind() const override {}
    void Unbind() const override {}

    void SetData(const void*, uint32_t) override {}

    const BufferLayout& GetLayout() const override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
    BufferLayout m_Layout;
};

class VulkanIndexBuffer : public IndexBuffer {
public:
    VulkanIndexBuffer(uint32_t* /*indices*/, uint32_t count);
    ~VulkanIndexBuffer() override = default;

    void Bind() const override {}
    void Unbind() const override {}

    uint32_t GetCount() const override { return m_Count; }

private:
    uint32_t m_Count = 0;
};

}  // namespace Vest
