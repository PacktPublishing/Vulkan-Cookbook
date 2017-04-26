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
// Recipe:  12 Creating a pipeline layout

#include "08 Graphics and Compute Pipelines/12 Creating a pipeline layout.h"

namespace VulkanCookbook {

  bool CreatePipelineLayout( VkDevice                                   logical_device,
                             std::vector<VkDescriptorSetLayout> const & descriptor_set_layouts,
                             std::vector<VkPushConstantRange> const   & push_constant_ranges,
                             VkPipelineLayout                         & pipeline_layout ) {
    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,          // VkStructureType                  sType
      nullptr,                                                // const void                     * pNext
      0,                                                      // VkPipelineLayoutCreateFlags      flags
      static_cast<uint32_t>(descriptor_set_layouts.size()),   // uint32_t                         setLayoutCount
      descriptor_set_layouts.data(),                          // const VkDescriptorSetLayout    * pSetLayouts
      static_cast<uint32_t>(push_constant_ranges.size()),     // uint32_t                         pushConstantRangeCount
      push_constant_ranges.data()                             // const VkPushConstantRange      * pPushConstantRanges
    };

    VkResult result = vkCreatePipelineLayout( logical_device, &pipeline_layout_create_info, nullptr, &pipeline_layout );

    if( VK_SUCCESS != result ) {
      std::cout << "Could not create pipeline layout." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
