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
// Recipe:  11 Creating a descriptor pool

#include "05 Descriptor Sets/11 Creating a descriptor pool.h"

namespace VulkanCookbook {

  bool CreateDescriptorPool( VkDevice                                  logical_device,
                             bool                                      free_individual_sets,
                             uint32_t                                  max_sets_count,
                             std::vector<VkDescriptorPoolSize> const & descriptor_types,
                             VkDescriptorPool                        & descriptor_pool ) {
    VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,                // VkStructureType                sType
      nullptr,                                                      // const void                   * pNext
      free_individual_sets ?                                        // VkDescriptorPoolCreateFlags    flags
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : 0u,
      max_sets_count,                                               // uint32_t                       maxSets
      static_cast<uint32_t>(descriptor_types.size()),               // uint32_t                       poolSizeCount
      descriptor_types.data()                                       // const VkDescriptorPoolSize   * pPoolSizes
    };

    VkResult result = vkCreateDescriptorPool( logical_device, &descriptor_pool_create_info, nullptr, &descriptor_pool );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create a descriptor pool." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
