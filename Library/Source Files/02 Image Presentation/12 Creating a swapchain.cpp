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
// Chapter: 02 Image Presentation
// Recipe:  12 Creating a swapchain

#include "02 Image Presentation/12 Creating a swapchain.h"

namespace VulkanCookbook {

  bool CreateSwapchain( VkDevice                        logical_device,
                        VkSurfaceKHR                    presentation_surface,
                        uint32_t                        image_count,
                        VkSurfaceFormatKHR              surface_format,
                        VkExtent2D                      image_size,
                        VkImageUsageFlags               image_usage,
                        VkSurfaceTransformFlagBitsKHR   surface_transform,
                        VkPresentModeKHR                present_mode,
                        VkSwapchainKHR                & old_swapchain,
                        VkSwapchainKHR                & swapchain ) {
    VkSwapchainCreateInfoKHR swapchain_create_info = {
      VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                  sType
      nullptr,                                      // const void                     * pNext
      0,                                            // VkSwapchainCreateFlagsKHR        flags
      presentation_surface,                         // VkSurfaceKHR                     surface
      image_count,                                  // uint32_t                         minImageCount
      surface_format.format,                        // VkFormat                         imageFormat
      surface_format.colorSpace,                    // VkColorSpaceKHR                  imageColorSpace
      image_size,                                   // VkExtent2D                       imageExtent
      1,                                            // uint32_t                         imageArrayLayers
      image_usage,                                  // VkImageUsageFlags                imageUsage
      VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                    imageSharingMode
      0,                                            // uint32_t                         queueFamilyIndexCount
      nullptr,                                      // const uint32_t                 * pQueueFamilyIndices
      surface_transform,                            // VkSurfaceTransformFlagBitsKHR    preTransform
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR      compositeAlpha
      present_mode,                                 // VkPresentModeKHR                 presentMode
      VK_TRUE,                                      // VkBool32                         clipped
      old_swapchain                                 // VkSwapchainKHR                   oldSwapchain
    };

    VkResult result = vkCreateSwapchainKHR( logical_device, &swapchain_create_info, nullptr, &swapchain );
    if( (VK_SUCCESS != result) ||
        (VK_NULL_HANDLE == swapchain) ) {
      std::cout << "Could not create a swapchain." << std::endl;
      return false;
    }

    if( VK_NULL_HANDLE != old_swapchain ) {
      vkDestroySwapchainKHR( logical_device, old_swapchain, nullptr );
      old_swapchain = VK_NULL_HANDLE;
    }

    return true;
  }

} // namespace VulkanCookbook
