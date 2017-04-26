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
// Recipe:  11 Selecting a format of swapchain images

#include "02 Image Presentation/11 Selecting a format of swapchain images.h"

namespace VulkanCookbook {

  bool SelectFormatOfSwapchainImages( VkPhysicalDevice     physical_device,
                                      VkSurfaceKHR         presentation_surface,
                                      VkSurfaceFormatKHR   desired_surface_format,
                                      VkFormat           & image_format,
                                      VkColorSpaceKHR    & image_color_space ) {
    // Enumerate supported formats
    uint32_t formats_count = 0;
    VkResult result = VK_SUCCESS;

    result = vkGetPhysicalDeviceSurfaceFormatsKHR( physical_device, presentation_surface, &formats_count, nullptr );
    if( (VK_SUCCESS != result) ||
        (0 == formats_count) ) {
      std::cout << "Could not get the number of supported surface formats." << std::endl;
      return false;
    }

    std::vector<VkSurfaceFormatKHR> surface_formats( formats_count );
    result = vkGetPhysicalDeviceSurfaceFormatsKHR( physical_device, presentation_surface, &formats_count, surface_formats.data() );
    if( (VK_SUCCESS != result) ||
        (0 == formats_count) ) {
      std::cout << "Could not enumerate supported surface formats." << std::endl;
      return false;
    }

    // Select surface format
    if( (1 == surface_formats.size()) &&
        (VK_FORMAT_UNDEFINED == surface_formats[0].format) ) {
      image_format = desired_surface_format.format;
      image_color_space = desired_surface_format.colorSpace;
      return true;
    }

    for( auto & surface_format : surface_formats ) {
      if( (desired_surface_format.format == surface_format.format) &&
          (desired_surface_format.colorSpace == surface_format.colorSpace) ) {
        image_format = desired_surface_format.format;
        image_color_space = desired_surface_format.colorSpace;
        return true;
      }
    }

    for( auto & surface_format : surface_formats ) {
      if( (desired_surface_format.format == surface_format.format) ) {
        image_format = desired_surface_format.format;
        image_color_space = surface_format.colorSpace;
        std::cout << "Desired combination of format and colorspace is not supported. Selecting other colorspace." << std::endl;
        return true;
      }
    }

    image_format = surface_formats[0].format;
    image_color_space = surface_formats[0].colorSpace;
    std::cout << "Desired format is not supported. Selecting available format - colorspace combination." << std::endl;
    return true;
  }

} // namespace VulkanCookbook
