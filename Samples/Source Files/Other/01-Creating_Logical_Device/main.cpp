// MIT License
//
// Copyright( c ) 2017 Packt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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
