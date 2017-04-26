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
// Chapter: 01 Instance and Devices
// Recipe:  03 Connecting with a Vulkan Loader library

#include "01 Instance and Devices/03 Connecting with a Vulkan Loader library.h"

namespace VulkanCookbook {

  bool ConnectWithVulkanLoaderLibrary( LIBRARY_TYPE & vulkan_library ) {
#if defined _WIN32
    vulkan_library = LoadLibrary( "vulkan-1.dll" );
#elif defined __linux
    vulkan_library = dlopen( "libvulkan.so.1", RTLD_NOW );
#endif

    if( vulkan_library == nullptr ) {
      std::cout << "Could not connect with a Vulkan Runtime library." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
