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
// 02-Creating_Swapchain

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSampleBase {
  VkDestroyer(VkInstance)     Instance;
  VkPhysicalDevice            PhysicalDevice;
  VkDestroyer(VkSurfaceKHR)   PresentationSurface;
  VkDestroyer(VkDevice)       LogicalDevice;
  uint32_t                    GraphicsQueueFamilyIndex;
  uint32_t                    PresentQueueFamilyIndex;
  VkQueue                     GraphicsQueue;
  VkQueue                     PresentQueue;
  VkDestroyer(VkSwapchainKHR) Swapchain;
  std::vector<VkImage>        SwapchainImages;
  VkDestroyer(VkSemaphore)    ImageAcquiredSemaphore;
  VkDestroyer(VkSemaphore)    ReadyToPresentSemaphore;
  VkDestroyer(VkCommandPool)  CommandPool;
  VkCommandBuffer             CommandBuffer;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    // Instance creation
    if( !ConnectWithVulkanLoaderLibrary( VulkanLibrary ) ) {
      return false;
    }

    if( !LoadFunctionExportedFromVulkanLoaderLibrary( VulkanLibrary ) ) {
      return false;
    }

    if( !LoadGlobalLevelFunctions() ) {
      return false;
    }

    std::vector<char const *> instance_extensions;
    if( !CreateVulkanInstanceWithWsiExtensionsEnabled( instance_extensions, "Vulkan Cookbook", *Instance ) ) {
      return false;
    }

    if( !LoadInstanceLevelFunctions( *Instance, instance_extensions ) ) {
      return false;
    }

    // Presentation surface creation
    InitVkDestroyer( Instance, PresentationSurface );
    if( !CreatePresentationSurface( *Instance, window_parameters, *PresentationSurface ) ) {
      return false;
    }

    // Logical device creation
    std::vector<VkPhysicalDevice> physical_devices;
    EnumerateAvailablePhysicalDevices( *Instance, physical_devices );

    for( auto & physical_device : physical_devices ) {
      if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_GRAPHICS_BIT, GraphicsQueueFamilyIndex ) ) {
        continue;
      }

      if( !SelectQueueFamilyThatSupportsPresentationToGivenSurface( physical_device, *PresentationSurface, PresentQueueFamilyIndex ) ) {
        continue;
      }

      std::vector<QueueInfo> requested_queues = { { GraphicsQueueFamilyIndex, { 1.0f } } };
      if( GraphicsQueueFamilyIndex != PresentQueueFamilyIndex ) {
        requested_queues.push_back( { PresentQueueFamilyIndex, { 1.0f } } );
      }
      VkDestroyer(VkDevice) logical_device;
      std::vector<char const *> device_extensions;
      if( !CreateLogicalDeviceWithWsiExtensionsEnabled( physical_device, requested_queues, device_extensions, nullptr, *logical_device ) ) {
        continue;
      } else {
        if( !LoadDeviceLevelFunctions( *logical_device, device_extensions ) ) {
          continue;
        }
        PhysicalDevice = physical_device;
        LogicalDevice = std::move( logical_device );
        GetDeviceQueue( *LogicalDevice, GraphicsQueueFamilyIndex, 0, GraphicsQueue );
        GetDeviceQueue( *LogicalDevice, PresentQueueFamilyIndex, 0, PresentQueue );
        break;
      }
    }

    if( !LogicalDevice ) {
      return false;
    }

    // Swapchain creation
    if( !CreateSwapchain() ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ImageAcquiredSemaphore );
    if( !CreateSemaphore( *LogicalDevice, *ImageAcquiredSemaphore ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ReadyToPresentSemaphore );
    if( !CreateSemaphore( *LogicalDevice, *ReadyToPresentSemaphore ) ) {
      return false;
    }

    // Command buffers creation
    InitVkDestroyer( LogicalDevice, CommandPool );
    if( !CreateCommandPool( *LogicalDevice, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, GraphicsQueueFamilyIndex, *CommandPool ) ) {
      return false;
    }

    std::vector<VkCommandBuffer> command_buffers;
    if( !AllocateCommandBuffers( *LogicalDevice, *CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, command_buffers ) ) {
      return false;
    }
    CommandBuffer = command_buffers[0];
    return true;
  }

  bool CreateSwapchain() {
    WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );

    Ready = false;

    VkFormat swapchain_image_format;
    VkExtent2D swapchain_image_size;
    VkDestroyer(VkSwapchainKHR) old_swapchain = std::move( Swapchain );
    InitVkDestroyer( LogicalDevice, Swapchain );
    if( !CreateSwapchainWithR8G8B8A8FormatAndMailboxPresentMode( PhysicalDevice, *PresentationSurface, *LogicalDevice, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, swapchain_image_size, swapchain_image_format, *old_swapchain, *Swapchain, SwapchainImages ) ) {
      return false;
    }

    if( *Swapchain ) {
      Ready = true;
    }
    return true;
  }

  virtual bool Draw() override {
    WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );

    uint32_t image_index;
    if( !AcquireSwapchainImage( *LogicalDevice, *Swapchain, *ImageAcquiredSemaphore, VK_NULL_HANDLE, image_index ) ) {
      return false;
    }

    if( !BeginCommandBufferRecordingOperation( CommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    ImageTransition image_transition_before_drawing = {
      SwapchainImages[image_index],             // VkImage              Image
      0,                                        // VkAccessFlags        CurrentAccess
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        NewAccess
      VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             CurrentQueueFamily
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             NewQueueFamily
      VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
    };
    SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );

    // Put drawing code here
    // ...

    ImageTransition image_transition_before_present = {
      SwapchainImages[image_index],             // VkImage              Image
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        CurrentAccess
      VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        CurrentLayout
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             CurrentQueueFamily
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             NewQueueFamily
      VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
    };
    SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );

    if( !EndCommandBufferRecordingOperation( CommandBuffer ) ) {
      return false;
    }

    WaitSemaphoreInfo wait_semaphore_info = {
      *ImageAcquiredSemaphore,            // VkSemaphore            Semaphore
      VK_PIPELINE_STAGE_ALL_COMMANDS_BIT  // VkPipelineStageFlags   WaitingStage
    };
    if( !SubmitCommandBuffersToQueue( PresentQueue, { wait_semaphore_info }, { CommandBuffer }, { *ReadyToPresentSemaphore }, VK_NULL_HANDLE ) ) {
      return false;
    }

    PresentInfo present_info = {
      *Swapchain,   // VkSwapchainKHR   Swapchain
      image_index   // uint32_t         ImageIndex
    };
    if( !PresentImage( PresentQueue, { *ReadyToPresentSemaphore }, { present_info } ) ) {
      return false;
    }
    return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain() ) {
      return false;
    }
    return true;
  }

  virtual void Deinitialize() override {
    if( LogicalDevice ) {
      WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );
    }
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "02 - Creating Swapchain", 50, 25, 1280, 800, Sample )
