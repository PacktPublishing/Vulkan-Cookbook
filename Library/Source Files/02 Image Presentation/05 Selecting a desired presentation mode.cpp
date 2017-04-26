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
// Recipe:  05 Selecting a desired presentation mode

#include "02 Image Presentation/05 Selecting a desired presentation mode.h"

namespace VulkanCookbook {

  bool SelectDesiredPresentationMode( VkPhysicalDevice   physical_device,
                                      VkSurfaceKHR       presentation_surface,
                                      VkPresentModeKHR   desired_present_mode,
                                      VkPresentModeKHR & present_mode ) {
    // Enumerate supported present modes
    uint32_t present_modes_count = 0;
    VkResult result = VK_SUCCESS;

    result = vkGetPhysicalDeviceSurfacePresentModesKHR( physical_device, presentation_surface, &present_modes_count, nullptr );
    if( (VK_SUCCESS != result) ||
        (0 == present_modes_count) ) {
      std::cout << "Could not get the number of supported present modes." << std::endl;
      return false;
    }

    std::vector<VkPresentModeKHR> present_modes( present_modes_count );
    result = vkGetPhysicalDeviceSurfacePresentModesKHR( physical_device, presentation_surface, &present_modes_count, present_modes.data() );
    if( (VK_SUCCESS != result) ||
      (0 == present_modes_count) ) {
      std::cout << "Could not enumerate present modes." << std::endl;
      return false;
    }

    // Select present mode
    for( auto & current_present_mode : present_modes ) {
      if( current_present_mode == desired_present_mode ) {
        present_mode = desired_present_mode;
        return true;
      }
    }

    std::cout << "Desired present mode is not supported. Selecting default FIFO mode." << std::endl;
    for( auto & current_present_mode : present_modes ) {
      if( current_present_mode == VK_PRESENT_MODE_FIFO_KHR ) {
        present_mode = VK_PRESENT_MODE_FIFO_KHR;
        return true;
      }
    }

    std::cout << "VK_PRESENT_MODE_FIFO_KHR is not supported though it's mandatory for all drivers!" << std::endl;
    return false;
  }

} // namespace VulkanCookbook
