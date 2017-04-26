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
// Recipe:  07 Specifying pipeline rasterization state

#include "08 Graphics and Compute Pipelines/07 Specifying pipeline rasterization state.h"

namespace VulkanCookbook {

  void SpecifyPipelineRasterizationState( bool                                     depth_clamp_enable,
                                          bool                                     rasterizer_discard_enable,
                                          VkPolygonMode                            polygon_mode,
                                          VkCullModeFlags                          culling_mode,
                                          VkFrontFace                              front_face,
                                          bool                                     depth_bias_enable,
                                          float                                    depth_bias_constant_factor,
                                          float                                    depth_bias_clamp,
                                          float                                    depth_bias_slope_factor,
                                          float                                    line_width,
                                          VkPipelineRasterizationStateCreateInfo & rasterization_state_create_info ) {
    rasterization_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // VkStructureType                            sType
      nullptr,                                                    // const void                               * pNext
      0,                                                          // VkPipelineRasterizationStateCreateFlags    flags
      depth_clamp_enable,                                         // VkBool32                                   depthClampEnable
      rasterizer_discard_enable,                                  // VkBool32                                   rasterizerDiscardEnable
      polygon_mode,                                               // VkPolygonMode                              polygonMode
      culling_mode,                                               // VkCullModeFlags                            cullMode
      front_face,                                                 // VkFrontFace                                frontFace
      depth_bias_enable,                                          // VkBool32                                   depthBiasEnable
      depth_bias_constant_factor,                                 // float                                      depthBiasConstantFactor
      depth_bias_clamp,                                           // float                                      depthBiasClamp
      depth_bias_slope_factor,                                    // float                                      depthBiasSlopeFactor
      line_width                                                  // float                                      lineWidth
    };
  }

} // namespace VulkanCookbook
