// Copyright Take Vos 2020-2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "gfx_pipeline_tone_mapper_vulkan.hpp"
#include "gfx_surface_vulkan.hpp"
#include "gfx_device_vulkan_impl.hpp"
#include "draw_context.hpp"
#include "../macros.hpp"

namespace hi { inline namespace v1 {

inline void gfx_pipeline_tone_mapper::draw_in_command_buffer(vk::CommandBuffer commandBuffer, draw_context const& context)
{
    gfx_pipeline::draw_in_command_buffer(commandBuffer, context);

    hi_axiom_not_null(device());
    device()->tone_mapper_pipeline->drawInCommandBuffer(commandBuffer);

    _push_constants.saturation = context.saturation;
    commandBuffer.pushConstants(pipelineLayout, vk::ShaderStageFlagBits::eFragment, 0, sizeof(push_constants), &_push_constants);

    device()->cmdBeginDebugUtilsLabelEXT(commandBuffer, "tone mapping");
    commandBuffer.draw(3, 1, 0, 0);
    device()->cmdEndDebugUtilsLabelEXT(commandBuffer);
}

inline std::vector<vk::PipelineShaderStageCreateInfo> gfx_pipeline_tone_mapper::createShaderStages() const
{
    hi_axiom_not_null(device());
    return device()->tone_mapper_pipeline->shaderStages;
}

inline std::vector<vk::DescriptorSetLayoutBinding> gfx_pipeline_tone_mapper::createDescriptorSetLayoutBindings() const
{
    return {
        {0, // binding
         vk::DescriptorType::eInputAttachment,
         1, // descriptorCount
         vk::ShaderStageFlagBits::eFragment}};
}

inline std::vector<vk::WriteDescriptorSet> gfx_pipeline_tone_mapper::createWriteDescriptorSet() const
{
    return {{
        descriptorSet,
        0, // destBinding
        0, // arrayElement
        1, // descriptorCount
        vk::DescriptorType::eInputAttachment,
        &surface->colorDescriptorImageInfos[0],
        nullptr, // bufferInfo
        nullptr // texelBufferView
    }};
}

inline size_t gfx_pipeline_tone_mapper::getDescriptorSetVersion() const
{
    return 1;
}

inline std::vector<vk::PushConstantRange> gfx_pipeline_tone_mapper::createPushConstantRanges() const
{
    return push_constants::pushConstantRanges();
}

inline vk::PipelineDepthStencilStateCreateInfo gfx_pipeline_tone_mapper::getPipelineDepthStencilStateCreateInfo() const
{
    // No depth buffering in the Tone Mapper
    return {
        vk::PipelineDepthStencilStateCreateFlags(),
        VK_FALSE, // depthTestEnable;
        VK_FALSE, // depthWriteEnable;
        vk::CompareOp::eAlways, // depthCompareOp
        VK_FALSE, // depthBoundsTestEnable
        VK_FALSE, // stencilTestEnable,
        vk::StencilOpState(), // front
        vk::StencilOpState(), // back
        0.0f, // minDepthBounds
        1.0f, // maxDepthBounds
    };
}

inline gfx_pipeline_tone_mapper::device_shared::device_shared(gfx_device const &device) : device(device)
{
    buildShaders();
}

inline gfx_pipeline_tone_mapper::device_shared::~device_shared() {}

inline void gfx_pipeline_tone_mapper::device_shared::destroy(gfx_device const *vulkanDevice)
{
    hi_assert_not_null(vulkanDevice);

    teardownShaders(vulkanDevice);
}

inline void gfx_pipeline_tone_mapper::device_shared::drawInCommandBuffer(vk::CommandBuffer const &commandBuffer)
{
    commandBuffer.bindIndexBuffer(device.quadIndexBuffer, 0, vk::IndexType::eUint16);
}

inline void gfx_pipeline_tone_mapper::device_shared::buildShaders()
{
    vertexShaderModule = device.loadShader(URL("resource:tone_mapper_vulkan.vert.spv"));
    fragmentShaderModule = device.loadShader(URL("resource:tone_mapper_vulkan.frag.spv"));

    shaderStages = {
        {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main"},
        {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main"}};
}

inline void gfx_pipeline_tone_mapper::device_shared::teardownShaders(gfx_device const*vulkanDevice)
{
    hi_assert_not_null(vulkanDevice);

    vulkanDevice->destroy(vertexShaderModule);
    vulkanDevice->destroy(fragmentShaderModule);
}

}} // namespace hi::inline v1::gfx_pipeline_tone_mapper
