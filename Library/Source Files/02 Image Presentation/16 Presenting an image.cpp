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
// Recipe:  16 Presenting an image

#include "02 Image Presentation/16 Presenting an image.h"

namespace VulkanCookbook {

  bool PresentImage( VkQueue                  queue,
                     std::vector<VkSemaphore> rendering_semaphores,
                     std::vector<PresentInfo> images_to_present ) {
    VkResult result;
    std::vector<VkSwapchainKHR> swapchains;
    std::vector<uint32_t> image_indices;

    for( auto & image_to_present : images_to_present ) {
      swapchains.emplace_back( image_to_present.Swapchain );
      image_indices.emplace_back( image_to_present.ImageIndex );
    }

    VkPresentInfoKHR present_info = {
      VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,                   // VkStructureType          sType
      nullptr,                                              // const void*              pNext
      static_cast<uint32_t>(rendering_semaphores.size()),   // uint32_t                 waitSemaphoreCount
      rendering_semaphores.data(),                          // const VkSemaphore      * pWaitSemaphores
      static_cast<uint32_t>(swapchains.size()),             // uint32_t                 swapchainCount
      swapchains.data(),                                    // const VkSwapchainKHR   * pSwapchains
      image_indices.data(),                                 // const uint32_t         * pImageIndices
      nullptr                                               // VkResult*                pResults
    };

    result = vkQueuePresentKHR( queue, &present_info );
    switch( result ) {
    case VK_SUCCESS:
      return true;
    default:
      return false;
    }
  }

} // namespace VulkanCookbook
