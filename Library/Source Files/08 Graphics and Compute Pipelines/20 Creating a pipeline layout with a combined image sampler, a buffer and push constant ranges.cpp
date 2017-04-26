// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and / or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The below copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// Vulkan Cookbook
// ISBN: 9781786468154
// © Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Chapter: 08 Graphics and Compute Pipelines
// Recipe:  20 Creating a pipeline layout with a combined image sampler, a buffer and push constant ranges

#include "05 Descriptor Sets/10 Creating a descriptor set layout.h"
#include "08 Graphics and Compute Pipelines/12 Creating a pipeline layout.h"
#include "08 Graphics and Compute Pipelines/20 Creating a pipeline layout with a combined image sampler, a buffer and push constant ranges.h"

namespace VulkanCookbook {

  bool CreatePipelineLayoutWithCombinedImageSamplerBufferAndPushConstantRanges( VkDevice                                 logical_device,
                                                                                std::vector<VkPushConstantRange> const & push_constant_ranges,
                                                                                VkDescriptorSetLayout                  & descriptor_set_layout,
                                                                                VkPipelineLayout                       & pipeline_layout ) {
    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings = {
      {
        0,                                  // uint32_t               binding
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,   // VkDescriptorType       descriptorType
        1,                                  // uint32_t               descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,       // VkShaderStageFlags     stageFlags
        nullptr                             // const VkSampler      * pImmutableSamplers
      },
      {
        1,                                  // uint32_t               binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  // VkDescriptorType       descriptorType
        1,                                  // uint32_t               descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT,         // VkShaderStageFlags     stageFlags
        nullptr                             // const VkSampler      * pImmutableSamplers
      }
    };

    if( !CreateDescriptorSetLayout( logical_device, descriptor_set_layout_bindings, descriptor_set_layout ) ) {
      return false;
    }

    if( !CreatePipelineLayout( logical_device, { descriptor_set_layout }, push_constant_ranges, pipeline_layout ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
