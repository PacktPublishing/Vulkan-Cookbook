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
// Recipe:  18 Creating a compute pipeline

#include "08 Graphics and Compute Pipelines/18 Creating a compute pipeline.h"

namespace VulkanCookbook {

  bool CreateComputePipeline( VkDevice                                logical_device,
                              VkPipelineCreateFlags                   additional_options,
                              VkPipelineShaderStageCreateInfo const & compute_shader_stage,
                              VkPipelineLayout                        pipeline_layout,
                              VkPipeline                              base_pipeline_handle,
                              VkPipelineCache                         pipeline_cache,
                              VkPipeline                            & compute_pipeline ) {
    VkComputePipelineCreateInfo compute_pipeline_create_info = {
      VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,   // VkStructureType                    sType
      nullptr,                                          // const void                       * pNext
      additional_options,                               // VkPipelineCreateFlags              flags
      compute_shader_stage,                             // VkPipelineShaderStageCreateInfo    stage
      pipeline_layout,                                  // VkPipelineLayout                   layout
      base_pipeline_handle,                             // VkPipeline                         basePipelineHandle
      -1                                                // int32_t                            basePipelineIndex
    };

    VkResult result = vkCreateComputePipelines( logical_device, pipeline_cache, 1, &compute_pipeline_create_info, nullptr, &compute_pipeline );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create compute pipeline." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
