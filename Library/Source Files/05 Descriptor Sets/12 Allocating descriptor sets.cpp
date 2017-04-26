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
// Chapter: 05 Descriptor Sets
// Recipe:  12 Allocating descriptor sets

#include "05 Descriptor Sets/12 Allocating descriptor sets.h"

namespace VulkanCookbook {

  bool AllocateDescriptorSets( VkDevice                                   logical_device,
                               VkDescriptorPool                           descriptor_pool,
                               std::vector<VkDescriptorSetLayout> const & descriptor_set_layouts,
                               std::vector<VkDescriptorSet>             & descriptor_sets ) {
    if( descriptor_set_layouts.size() > 0 ) {
      VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,         // VkStructureType                  sType
        nullptr,                                                // const void                     * pNext
        descriptor_pool,                                        // VkDescriptorPool                 descriptorPool
        static_cast<uint32_t>(descriptor_set_layouts.size()),   // uint32_t                         descriptorSetCount
        descriptor_set_layouts.data()                           // const VkDescriptorSetLayout    * pSetLayouts
      };

      descriptor_sets.resize( descriptor_set_layouts.size() );

      VkResult result = vkAllocateDescriptorSets( logical_device, &descriptor_set_allocate_info, descriptor_sets.data() );
      if( VK_SUCCESS != result ) {
        std::cout << "Could not allocate descriptor sets." << std::endl;
        return false;
      }
      return true;
    }
    return false;
  }

} // namespace VulkanCookbook
