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
// 01-Creating_Logical_Device

#include "CookbookSampleFramework.h"

#include "01 Instance and Devices\03 Connecting with a Vulkan Loader library.h"
#include "01 Instance and Devices\05 Loading function exported from a Vulkan Loader library.h"
#include "01 Instance and Devices\06 Loading global-level functions.h"
#include "01 Instance and Devices\08 Creating a Vulkan Instance.h"
#include "01 Instance and Devices\09 Loading instance-level functions.h"
#include "01 Instance and Devices\18 Creating a logical device with geometry shaders and graphics queue.h"
#include "01 Instance and Devices\19 Destroying a logical device.h"
#include "01 Instance and Devices\20 Destroying a Vulkan Instance.h"
#include "01 Instance and Devices\21 Releasing a Vulkan Loader library.h"
#include "03 Command Buffers and Synchronization/15 Waiting for all submitted commands to be finished.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSampleBase {
  LIBRARY_TYPE            VulkanLibrary;
  VkDestroyer<VkInstance> Instance;
  VkDestroyer<VkDevice>   LogicalDevice;
  VkQueue                 GraphicsQueue;
  VkQueue                 ComputeQueue;

  virtual bool Initialize( WindowParameters  window_parameters ) override {
    if( !ConnectWithVulkanLoaderLibrary( VulkanLibrary ) ) {
      return false;
    }

    if( !LoadFunctionExportedFromVulkanLoaderLibrary( VulkanLibrary ) ) {
      return false;
    }

    if( !LoadGlobalLevelFunctions() ) {
      return false;
    }

    if( !CreateVulkanInstance( {}, "Vulkan Cookbook", *Instance ) ) {
      return false;
    }

    if( !LoadInstanceLevelFunctions( *Instance, {} ) ) {
      return false;
    }

    if( !CreateLogicalDeviceWithGeometryShadersAndGraphicsAndComputeQueues( *Instance, *LogicalDevice, GraphicsQueue, ComputeQueue ) ) {
      return false;
    }

    return true;
  }

  virtual bool Draw() override {
    return true;
  }

  virtual bool Resize() override {
    return true;
  }

  virtual void Deinitialize() override {
    if( LogicalDevice ) {
      WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );
    }
  }
};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "01 - Creating Logical Device", 50, 25, 1280, 800, Sample )
