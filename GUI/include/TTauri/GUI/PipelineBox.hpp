// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "TTauri/GUI/Pipeline_vulkan.hpp"
#include "TTauri/GUI/PipelineBox_PushConstants.hpp"
#include "TTauri/GUI/PipelineBox_Vertex.hpp"
#include "TTauri/GUI/globals.hpp"
#include "TTauri/GUI/Device_forward.hpp"
#include "TTauri/Foundation/vspan.hpp"
#include <vma/vk_mem_alloc.h>
#include <gsl/gsl>

namespace TTauri::GUI::PipelineBox {

/*! Pipeline for rendering simple box shaded quats.
 */
class PipelineBox : public Pipeline_vulkan {
public:
    static constexpr int maximumNumberOfVertices = 65536;
    static constexpr int maximumNumberOfSquares = maximumNumberOfVertices / 4;
    static constexpr int maximumNumberOfTriangles = maximumNumberOfSquares * 2;
    static constexpr int maximumNumberOfIndices = maximumNumberOfTriangles * 3;
    
    vspan<Vertex> vertexBufferData;

    PipelineBox(Window const &window);
    ~PipelineBox() {};

    PipelineBox(const PipelineBox &) = delete;
    PipelineBox &operator=(const PipelineBox &) = delete;
    PipelineBox(PipelineBox &&) = delete;
    PipelineBox &operator=(PipelineBox &&) = delete;

    vk::Semaphore render(vk::Framebuffer frameBuffer, vk::Semaphore inputSemaphore) override;

protected:
    PushConstants pushConstants;

    vk::Buffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;

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