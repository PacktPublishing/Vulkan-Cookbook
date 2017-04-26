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
// Chapter: 04 Resources and Memory
// Recipe:  02 Allocating and binding memory object to a buffer

#include "04 Resources and Memory/02 Allocating and binding memory object to a buffer.h"

namespace VulkanCookbook {

  bool AllocateAndBindMemoryObjectToBuffer( VkPhysicalDevice           physical_device,
                                            VkDevice                   logical_device,
                                            VkBuffer                   buffer,
                                            VkMemoryPropertyFlagBits   memory_properties,
                                            VkDeviceMemory           & memory_object ) {
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    vkGetPhysicalDeviceMemoryProperties( physical_device, &physical_device_memory_properties );

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements( logical_device, buffer, &memory_requirements );

    memory_object = VK_NULL_HANDLE;
    for( uint32_t type = 0; type < physical_device_memory_properties.memoryTypeCount; ++type ) {
      if( (memory_requirements.memoryTypeBits & (1 << type)) &&
          ((physical_device_memory_properties.memoryTypes[type].propertyFlags & memory_properties) == memory_properties) ) {

        VkMemoryAllocateInfo buffer_memory_allocate_info = {
          VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,   // VkStructureType    sType
          nullptr,                                  // const void       * pNext
          memory_requirements.size,                 // VkDeviceSize       allocationSize
          type                                      // uint32_t           memoryTypeIndex
        };

        VkResult result = vkAllocateMemory( logical_device, &buffer_memory_allocate_info, nullptr, &memory_object );
        if( VK_SUCCESS == result ) {
          break;
        }
      }
    }

    if( VK_NULL_HANDLE == memory_object ) {
      std::cout << "Could not allocate memory for a buffer." << std::endl;
      return false;
    }

    VkResult result = vkBindBufferMemory( logical_device, buffer, memory_object, 0 );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not bind memory object to a buffer." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
