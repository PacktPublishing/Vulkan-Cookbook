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
// Recipe:  11 Mapping, updating and unmapping host-visible memory

#include "04 Resources and Memory/11 Mapping, updating and unmapping host-visible memory.h"

namespace VulkanCookbook {

  bool MapUpdateAndUnmapHostVisibleMemory( VkDevice             logical_device,
                                           VkDeviceMemory       memory_object,
                                           VkDeviceSize         offset,
                                           VkDeviceSize         data_size,
                                           void               * data,
                                           bool                 unmap,
                                           void             * * pointer ) {
    VkResult result;
    void * local_pointer;
    result = vkMapMemory( logical_device, memory_object, offset, data_size, 0, &local_pointer );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not map memory object." << std::endl;
      return false;
    }

    std::memcpy( local_pointer, data, data_size );

    std::vector<VkMappedMemoryRange> memory_ranges = {
      {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,  // VkStructureType    sType
        nullptr,                                // const void       * pNext
        memory_object,                          // VkDeviceMemory     memory
        offset,                                 // VkDeviceSize       offset
        VK_WHOLE_SIZE                           // VkDeviceSize       size
      }
    };

    vkFlushMappedMemoryRanges( logical_device, static_cast<uint32_t>(memory_ranges.size()), memory_ranges.data() );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not flush mapped memory." << std::endl;
      return false;
    }

    if( unmap ) {
      vkUnmapMemory( logical_device, memory_object );
    } else if( nullptr != pointer ) {
      *pointer = local_pointer;
    }

    return true;
  }

} // namespace VulkanCookbook
