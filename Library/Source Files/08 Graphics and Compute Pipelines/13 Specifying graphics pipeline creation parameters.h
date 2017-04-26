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
// Recipe:  13 Specifying graphics pipeline creation parameters

#ifndef SPECIFYING_GRAPHICS_PIPELINE_CREATION_PARAMETERS
#define SPECIFYING_GRAPHICS_PIPELINE_CREATION_PARAMETERS

#include "Common.h"

namespace VulkanCookbook {

  void SpecifyGraphicsPipelineCreationParameters( VkPipelineCreateFlags                                additional_options,
                                                  std::vector<VkPipelineShaderStageCreateInfo> const & shader_stage_create_infos,
                                                  VkPipelineVertexInputStateCreateInfo const         & vertex_input_state_create_info,
                                                  VkPipelineInputAssemblyStateCreateInfo const       & input_assembly_state_create_info,
                                                  VkPipelineTessellationStateCreateInfo const        * tessellation_state_create_info,
                                                  VkPipelineViewportStateCreateInfo const            * viewport_state_create_info,
                                                  VkPipelineRasterizationStateCreateInfo const       & rasterization_state_create_info,
                                                  VkPipelineMultisampleStateCreateInfo const         * multisample_state_create_info,
                                                  VkPipelineDepthStencilStateCreateInfo const        * depth_and_stencil_state_create_info,
                                                  VkPipelineColorBlendStateCreateInfo const          * blend_state_create_info,
                                                  VkPipelineDynamicStateCreateInfo const             * dynamic_state_creat_info,
                                                  VkPipelineLayout                                     pipeline_layout,
                                                  VkRenderPass                                         render_pass,
                                                  uint32_t                                             subpass,
                                                  VkPipeline                                           base_pipeline_handle,
                                                  int32_t                                              base_pipeline_index,
                                                  VkGraphicsPipelineCreateInfo                       & graphics_pipeline_create_info );

} // namespace VulkanCookbook

#endif // SPECIFYING_GRAPHICS_PIPELINE_CREATION_PARAMETERS