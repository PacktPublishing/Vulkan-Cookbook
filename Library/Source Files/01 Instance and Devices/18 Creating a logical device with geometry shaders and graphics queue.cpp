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
// Recipe:  18 Creating a logical device with geometry shaders and graphics and compute queues

#include "01 Instance and Devices/10 Enumerating available physical devices.h"
#include "01 Instance and Devices/12 Getting features and properties of a physical device.h"
#include "01 Instance and Devices/14 Selecting index of a queue family with desired capabilities.h"
#include "01 Instance and Devices/15 Creating a logical device.h"
#include "01 Instance and Devices/16 Loading device-level functions.h"
#include "01 Instance and Devices/17 Getting a device queue.h"

namespace VulkanCookbook {

  bool CreateLogicalDeviceWithGeometryShadersAndGraphicsAndComputeQueues( VkInstance   instance,
                                                                          VkDevice   & logical_device,
                                                                          VkQueue    & graphics_queue,
                                                                          VkQueue    & compute_queue ) {
    std::vector<VkPhysicalDevice> physical_devices;
    EnumerateAvailablePhysicalDevices( instance, physical_devices );

    for( auto & physical_device : physical_devices ) {
      VkPhysicalDeviceFeatures device_features;
      VkPhysicalDeviceProperties device_properties;
      GetFeaturesAndPropertiesOfPhysicalDevice( physical_device, device_features, device_properties );

      if( !device_features.geometryShader ) {
        continue;
      } else {
        device_features = {};
        device_features.geometryShader = VK_TRUE;
      }

      uint32_t graphics_queue_family_index;
      if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_GRAPHICS_BIT, graphics_queue_family_index ) ) {
        continue;
      }

      uint32_t compute_queue_family_index;
      if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_COMPUTE_BIT, compute_queue_family_index ) ) {
        continue;
      }

      std::vector<QueueInfo> requested_queues = { { graphics_queue_family_index, { 1.0f } } };
      if( graphics_queue_family_index != compute_queue_family_index ) {
        requested_queues.push_back( { compute_queue_family_index, { 1.0f } } );
      }

      if( !CreateLogicalDevice( physical_device, requested_queues, {}, &device_features, logical_device ) ) {
        continue;
      } else {
        if( !LoadDeviceLevelFunctions( logical_device, {} ) ) {
          return false;
        }
        GetDeviceQueue( logical_device, graphics_queue_family_index, 0, graphics_queue );
        GetDeviceQueue( logical_device, compute_queue_family_index, 0, compute_queue );
        return true;
      }
    }
    return false;
  }

} // namespace VulkanCookbook
