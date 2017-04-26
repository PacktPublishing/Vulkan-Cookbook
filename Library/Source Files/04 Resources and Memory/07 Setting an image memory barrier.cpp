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
// Recipe:  07 Setting an image memory barrier

#include "04 Resources and Memory/07 Setting an image memory barrier.h"

namespace VulkanCookbook {

  void SetImageMemoryBarrier( VkCommandBuffer              command_buffer,
                              VkPipelineStageFlags         generating_stages,
                              VkPipelineStageFlags         consuming_stages,
                              std::vector<ImageTransition> image_transitions ) {
    std::vector<VkImageMemoryBarrier> image_memory_barriers;

    for( auto & image_transition : image_transitions ) {
      image_memory_barriers.push_back( {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,   // VkStructureType            sType
        nullptr,                                  // const void               * pNext
        image_transition.CurrentAccess,           // VkAccessFlags              srcAccessMask
        image_transition.NewAccess,               // VkAccessFlags              dstAccessMask
        image_transition.CurrentLayout,           // VkImageLayout              oldLayout
        image_transition.NewLayout,               // VkImageLayout              newLayout
        image_transition.CurrentQueueFamily,      // uint32_t                   srcQueueFamilyIndex
        image_transition.NewQueueFamily,          // uint32_t                   dstQueueFamilyIndex
        image_transition.Image,                   // VkImage                    image
        {                                         // VkImageSubresourceRange    subresourceRange
          image_transition.Aspect,                  // VkImageAspectFlags         aspectMask
          0,                                        // uint32_t                   baseMipLevel
          VK_REMAINING_MIP_LEVELS,                  // uint32_t                   levelCount
          0,                                        // uint32_t                   baseArrayLayer
          VK_REMAINING_ARRAY_LAYERS                 // uint32_t                   layerCount
        }
      } );
    }

    if( image_memory_barriers.size() > 0 ) {
      vkCmdPipelineBarrier( command_buffer, generating_stages, consuming_stages, 0, 0, nullptr, 0, nullptr, static_cast<uint32_t>(image_memory_barriers.size()), image_memory_barriers.data() );
    }
  }

} // namespace VulkanCookbook
