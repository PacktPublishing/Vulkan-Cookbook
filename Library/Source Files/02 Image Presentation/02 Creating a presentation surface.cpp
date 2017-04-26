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
// Recipe:  02 Creating a presentation surface

#include "02 Image Presentation/02 Creating a presentation surface.h"

namespace VulkanCookbook {

  bool CreatePresentationSurface( VkInstance         instance,
                                  WindowParameters   window_parameters,
                                  VkSurfaceKHR     & presentation_surface ) {
    VkResult result;

#ifdef VK_USE_PLATFORM_WIN32_KHR

    VkWin32SurfaceCreateInfoKHR surface_create_info = {
      VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                 sType
      nullptr,                                          // const void                    * pNext
      0,                                                // VkWin32SurfaceCreateFlagsKHR    flags
      window_parameters.HInstance,                      // HINSTANCE                       hinstance
      window_parameters.HWnd                            // HWND                            hwnd
    };

    result = vkCreateWin32SurfaceKHR( instance, &surface_create_info, nullptr, &presentation_surface );

#elif defined VK_USE_PLATFORM_XLIB_KHR

    VkXlibSurfaceCreateInfoKHR surface_create_info = {
      VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                 sType
      nullptr,                                          // const void                    * pNext
      0,                                                // VkXlibSurfaceCreateFlagsKHR     flags
      window_parameters.Dpy,                            // Display                       * dpy
      window_parameters.Window                          // Window                          window
    };

    result = vkCreateXlibSurfaceKHR( instance, &surface_create_info, nullptr, &presentation_surface );

#elif defined VK_USE_PLATFORM_XCB_KHR

    VkXcbSurfaceCreateInfoKHR surface_create_info = {
      VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                 sType
      nullptr,                                          // const void                    * pNext
      0,                                                // VkXcbSurfaceCreateFlagsKHR      flags
      window_parameters.Connection,                     // xcb_connection_t              * connection
      window_parameters.Window                          // xcb_window_t                    window
    };

    result = vkCreateXcbSurfaceKHR( instance, &surface_create_info, nullptr, &presentation_surface );

#endif

    if( (VK_SUCCESS != result) ||
        (VK_NULL_HANDLE == presentation_surface) ) {
      std::cout << "Could not create presentation surface." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
