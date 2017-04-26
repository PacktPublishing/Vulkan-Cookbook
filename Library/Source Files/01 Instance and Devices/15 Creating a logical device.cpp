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
// Recipe:  15 Creating a logical device

#include "01 Instance and Devices/11 Checking available device extensions.h"
#include "01 Instance and Devices/15 Creating a logical device.h"

namespace VulkanCookbook {

  bool CreateLogicalDevice( VkPhysicalDevice                  physical_device,
                            std::vector<QueueInfo>            queue_infos,
                            std::vector<char const *> const & desired_extensions,
                            VkPhysicalDeviceFeatures        * desired_features,
                            VkDevice                        & logical_device ) {
    std::vector<VkExtensionProperties> available_extensions;
    if( !CheckAvailableDeviceExtensions( physical_device, available_extensions ) ) {
      return false;
    }

    for( auto & extension : desired_extensions ) {
      if( !IsExtensionSupported( available_extensions, extension ) ) {
        std::cout << "Extension named '" << extension << "' is not supported by a physical device." << std::endl;
        return false;
      }
    }

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    for( auto & info : queue_infos ) {
      queue_create_infos.push_back( {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType                  sType
        nullptr,                                          // const void                     * pNext
        0,                                                // VkDeviceQueueCreateFlags         flags
        info.FamilyIndex,                                 // uint32_t                         queueFamilyIndex
        static_cast<uint32_t>(info.Priorities.size()),    // uint32_t                         queueCount
        info.Priorities.data()                            // const float                    * pQueuePriorities
      } );
    };

    VkDeviceCreateInfo device_create_info = {
      VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // VkStructureType                  sType
      nullptr,                                            // const void                     * pNext
      0,                                                  // VkDeviceCreateFlags              flags
      static_cast<uint32_t>(queue_create_infos.size()),   // uint32_t                         queueCreateInfoCount
      queue_create_infos.data(),                          // const VkDeviceQueueCreateInfo  * pQueueCreateInfos
      0,                                                  // uint32_t                         enabledLayerCount
      nullptr,                                            // const char * const             * ppEnabledLayerNames
      static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                         enabledExtensionCount
      desired_extensions.data(),                          // const char * const             * ppEnabledExtensionNames
      desired_features                                    // const VkPhysicalDeviceFeatures * pEnabledFeatures
    };

    VkResult result = vkCreateDevice( physical_device, &device_create_info, nullptr, &logical_device );
    if( (result != VK_SUCCESS) ||
        (logical_device == VK_NULL_HANDLE) ) {
      std::cout << "Could not create logical device." << std::endl;
      return false;
    }

    return true;
  }

} // namespace VulkanCookbook
