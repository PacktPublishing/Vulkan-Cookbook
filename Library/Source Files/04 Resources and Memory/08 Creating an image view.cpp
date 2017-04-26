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
// Recipe:  08 Creating an image view

#include "04 Resources and Memory/08 Creating an image view.h"

namespace VulkanCookbook {

  bool CreateImageView( VkDevice             logical_device,
                        VkImage              image,
                        VkImageViewType      view_type,
                        VkFormat             format,
                        VkImageAspectFlags   aspect,
                        VkImageView        & image_view ) {
    VkImageViewCreateInfo image_view_create_info = {
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   // VkStructureType            sType
      nullptr,                                    // const void               * pNext
      0,                                          // VkImageViewCreateFlags     flags
      image,                                      // VkImage                    image
      view_type,                                  // VkImageViewType            viewType
      format,                                     // VkFormat                   format
      {                                           // VkComponentMapping         components
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         r
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         g
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         b
        VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle         a
      },
      {                                           // VkImageSubresourceRange    subresourceRange
        aspect,                                     // VkImageAspectFlags         aspectMask
        0,                                          // uint32_t                   baseMipLevel
        VK_REMAINING_MIP_LEVELS,                    // uint32_t                   levelCount
        0,                                          // uint32_t                   baseArrayLayer
        VK_REMAINING_ARRAY_LAYERS                   // uint32_t                   layerCount
      }
    };

    VkResult result = vkCreateImageView( logical_device, &image_view_create_info, nullptr, &image_view );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create an image view." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
