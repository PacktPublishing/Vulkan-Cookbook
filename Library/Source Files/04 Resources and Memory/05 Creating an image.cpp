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
// Recipe:  05 Creating an image

#include "04 Resources and Memory/05 Creating an image.h"

namespace VulkanCookbook {

  bool CreateImage( VkDevice                logical_device,
                    VkImageType             type,
                    VkFormat                format,
                    VkExtent3D              size,
                    uint32_t                num_mipmaps,
                    uint32_t                num_layers,
                    VkSampleCountFlagBits   samples,
                    VkImageUsageFlags       usage_scenarios,
                    bool                    cubemap,
                    VkImage               & image ) {
    VkImageCreateInfo image_create_info = {
      VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,                // VkStructureType          sType
      nullptr,                                            // const void             * pNext
      cubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u, // VkImageCreateFlags       flags
      type,                                               // VkImageType              imageType
      format,                                             // VkFormat                 format
      size,                                               // VkExtent3D               extent
      num_mipmaps,                                        // uint32_t                 mipLevels
      cubemap ? 6 * num_layers : num_layers,              // uint32_t                 arrayLayers
      samples,                                            // VkSampleCountFlagBits    samples
      VK_IMAGE_TILING_OPTIMAL,                            // VkImageTiling            tiling
      usage_scenarios,                                    // VkImageUsageFlags        usage
      VK_SHARING_MODE_EXCLUSIVE,                          // VkSharingMode            sharingMode
      0,                                                  // uint32_t                 queueFamilyIndexCount
      nullptr,                                            // const uint32_t         * pQueueFamilyIndices
      VK_IMAGE_LAYOUT_UNDEFINED                           // VkImageLayout            initialLayout
    };

    VkResult result = vkCreateImage( logical_device, &image_create_info, nullptr, &image );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create an image." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
