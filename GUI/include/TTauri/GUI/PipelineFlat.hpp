// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "TTauri/GUI/Pipeline_vulkan.hpp"
#include "TTauri/GUI/PipelineFlat_PushConstants.hpp"
#include "TTauri/GUI/PipelineFlat_Vertex.hpp"
#include "TTauri/GUI/globals.hpp"
#include "TTauri/GUI/Device_forward.hpp"
#include <vma/vk_mem_alloc.h>
#include <gsl/gsl>

namespace TTauri::GUI::PipelineFlat {

/*! Pipeline for rendering simple flat shaded quats.
 */
class PipelineFlat : public Pipeline_vulkan {
public:
    static constexpr int maximumNumberOfVertices = 65536;
    static constexpr int maximumNumberOfSquares = maximumNumberOfVertices / 4;
    static constexpr int maximumNumberOfTriangles = maximumNumberOfSquares * 2;
    static constexpr int maximumNumberOfIndices = maximumNumberOfTriangles * 3;
    
    PipelineFlat(Window const &window);
    ~PipelineFlat() {};

    PipelineFlat(const PipelineFlat &) = delete;
    PipelineFlat &operator=(const PipelineFlat &) = delete;
    PipelineFlat(PipelineFlat &&) = delete;
    PipelineFlat &operator=(PipelineFlat &&) = delete;

    vk::Semaphore render(vk::Framebuffer frameBuffer, vk::Semaphore inputSemaphore) override;

protected:
    PushConstants pushConstants;

    ssize_t numberOfVertices = 0;
    vk::Buffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;
    gsl::span<Vertex> vertexBufferData;

    void drawInCommandBuffer() override;

    std::vector<vk::PipelineShaderStageCreateInfo> createShaderStages() const override;
    std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings() const override;
    std::vector<vk::WriteDescriptorSet> createWriteDescriptorSet() const override;
    virtual ssize_t getDescriptorSetVersion() const override;
    std::vector<vk::PushConstantRange> createPushConstantRanges() const override;
    vk::VertexInputBindingDescription createVertexInputBindingDescription() const override;
    std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions() const override;

private:
    void buildVertexBuffers() override;
    void teardownVertexBuffers() override;
};

}
