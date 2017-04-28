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
// Cookbook Sample Framework

#include "CookbookSampleFramework.h"

namespace VulkanCookbook {

  MouseState::MouseState() {
    Buttons[0].IsPressed = false;
    Buttons[0].WasClicked = false;
    Buttons[0].WasRelease = false;
    Buttons[1].IsPressed = false;
    Buttons[1].WasClicked = false;
    Buttons[1].WasRelease = false;
    Position.X = 0;
    Position.Y = 0;
    Position.Delta.X = 0;
    Position.Delta.Y = 0;
    Wheel.WasMoved = false;
    Wheel.Distance = 0.0f;
  }

  MouseState::~MouseState() {
  }

  float TimerState::GetTime() const {
    auto duration = Time.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<float>(milliseconds * 0.001f);
  }

  float TimerState::GetDeltaTime() const {
    return DeltaTime.count();
  }

  void TimerState::Update() {
    auto previous_time = Time;
    Time = std::chrono::high_resolution_clock::now();
    DeltaTime = std::chrono::high_resolution_clock::now() - previous_time;
  }

  TimerState::TimerState() {
    Update();
  }

  TimerState::~TimerState() {
  }

  VulkanCookbookSampleBase::VulkanCookbookSampleBase() :
    VulkanLibrary( nullptr ),
    Ready( false ) {
  }

  VulkanCookbookSampleBase::~VulkanCookbookSampleBase() {
    ReleaseVulkanLoaderLibrary( VulkanLibrary );
  }

  void VulkanCookbookSampleBase::MouseClick( size_t button_index, bool state ) {
    if( 2 > button_index ) {
      MouseState.Buttons[button_index].IsPressed = state;
      MouseState.Buttons[button_index].WasClicked = state;
      MouseState.Buttons[button_index].WasRelease = !state;
      OnMouseEvent();
    }
  }

  void VulkanCookbookSampleBase::MouseMove( int x, int y ) {
    MouseState.Position.Delta.X = x - MouseState.Position.X;
    MouseState.Position.Delta.Y = y - MouseState.Position.Y;
    MouseState.Position.X = x;
    MouseState.Position.Y = y;

    OnMouseEvent();
  }

  void VulkanCookbookSampleBase::MouseWheel( float distance ) {
    MouseState.Wheel.WasMoved = true;
    MouseState.Wheel.Distance = distance;
    OnMouseEvent();
  }

  void VulkanCookbookSampleBase::MouseReset() {
    MouseState.Position.Delta.X = 0;
    MouseState.Position.Delta.Y = 0;
    MouseState.Buttons[0].WasClicked = false;
    MouseState.Buttons[0].WasRelease = false;
    MouseState.Buttons[1].WasClicked = false;
    MouseState.Buttons[1].WasRelease = false;
    MouseState.Wheel.WasMoved = false;
    MouseState.Wheel.Distance = 0.0f;
  }

  void VulkanCookbookSampleBase::UpdateTime() {
    TimerState.Update();
  }

  bool VulkanCookbookSampleBase::IsReady() {
    return Ready;
  }

  void VulkanCookbookSampleBase::OnMouseEvent() {
    // Override this in a derived class to know when a mouse event occured
  }

  bool VulkanCookbookSample::InitializeVulkan( WindowParameters           window_parameters,
                                               VkPhysicalDeviceFeatures * desired_device_features,
                                               VkImageUsageFlags          swapchain_image_usage,
                                               bool                       use_depth,
                                               VkImageUsageFlags          depth_attachment_usage ) {
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

    PresentationSurface = VkDestroyer<VkSurfaceKHR>( *Instance );
    if( !CreatePresentationSurface( *Instance, window_parameters, *PresentationSurface ) ) {
      return false;
    }

    std::vector<VkPhysicalDevice> physical_devices;
    EnumerateAvailablePhysicalDevices( *Instance, physical_devices );

    for( auto & physical_device : physical_devices ) {
      if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_GRAPHICS_BIT, GraphicsQueue.FamilyIndex ) ) {
        continue;
      }

      if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_COMPUTE_BIT, ComputeQueue.FamilyIndex ) ) {
        continue;
      }

      if( !SelectQueueFamilyThatSupportsPresentationToGivenSurface( physical_device, *PresentationSurface, PresentQueue.FamilyIndex ) ) {
        continue;
      }

      std::vector<QueueInfo> requested_queues = { { GraphicsQueue.FamilyIndex, { 1.0f } } };
      if( GraphicsQueue.FamilyIndex != ComputeQueue.FamilyIndex ) {
        requested_queues.push_back( { ComputeQueue.FamilyIndex,{ 1.0f } } );
      }
      if( (GraphicsQueue.FamilyIndex != PresentQueue.FamilyIndex) &&
          (ComputeQueue.FamilyIndex != PresentQueue.FamilyIndex) ) {
        requested_queues.push_back( { PresentQueue.FamilyIndex, { 1.0f } } );
      }
      std::vector<char const *> device_extensions;
      if( !CreateLogicalDeviceWithWsiExtensionsEnabled( physical_device, requested_queues, device_extensions, desired_device_features, *LogicalDevice ) ) {
        continue;
      } else {
        PhysicalDevice = physical_device;
        LoadDeviceLevelFunctions( *LogicalDevice, device_extensions );
        GetDeviceQueue( *LogicalDevice, GraphicsQueue.FamilyIndex, 0, GraphicsQueue.Handle );
        GetDeviceQueue( *LogicalDevice, GraphicsQueue.FamilyIndex, 0, ComputeQueue.Handle );
        GetDeviceQueue( *LogicalDevice, PresentQueue.FamilyIndex, 0, PresentQueue.Handle );
        break;
      }
    }

    if( !LogicalDevice ) {
      return false;
    }

    // Prepare frame resources

    InitVkDestroyer( *LogicalDevice, CommandPool );
    if( !CreateCommandPool( *LogicalDevice, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, GraphicsQueue.FamilyIndex, *CommandPool ) ) {
      return false;
    }

    for( uint32_t i = 0; i < FramesCount; ++i ) {
      std::vector<VkCommandBuffer> command_buffer;
      VkDestroyer<VkSemaphore> image_acquired_semaphore( LogicalDevice );
      VkDestroyer<VkSemaphore> ready_to_present_semaphore( LogicalDevice );
      VkDestroyer<VkFence> drawing_finished_fence( LogicalDevice );
      VkDestroyer<VkImageView> depth_attachment( LogicalDevice );

      if( !AllocateCommandBuffers( *LogicalDevice, *CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, command_buffer ) ) {
        return false;
      }
      if( !CreateSemaphore( *LogicalDevice, *image_acquired_semaphore ) ) {
        return false;
      }
      if( !CreateSemaphore( *LogicalDevice, *ready_to_present_semaphore ) ) {
        return false;
      }
      if( !CreateFence( *LogicalDevice, true, *drawing_finished_fence ) ) {
        return false;
      }

      FrameResources.push_back( {
        command_buffer[0],
        std::move( image_acquired_semaphore ),
        std::move( ready_to_present_semaphore ),
        std::move( drawing_finished_fence ),
        std::move( depth_attachment ),
        {}
      } );
    }

    if( !CreateSwapchain( swapchain_image_usage, use_depth, depth_attachment_usage ) ) {
      return false;
    }

    return true;
  }

  bool VulkanCookbookSample::CreateSwapchain( VkImageUsageFlags swapchain_image_usage,
                                              bool              use_depth,
                                              VkImageUsageFlags depth_attachment_usage ) {
    WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );

    Ready = false;

    Swapchain.ImageViewsRaw.clear();
    Swapchain.ImageViews.clear();
    Swapchain.Images.clear();

    VkDestroyer<VkSwapchainKHR> old_swapchain = std::move( Swapchain.Handle );
    Swapchain.Handle = VkDestroyer<VkSwapchainKHR>( *LogicalDevice );
    if( !CreateSwapchainWithR8G8B8A8FormatAndMailboxPresentMode( PhysicalDevice, *PresentationSurface, *LogicalDevice, swapchain_image_usage, Swapchain.Size, Swapchain.Format, *old_swapchain, *Swapchain.Handle, Swapchain.Images ) ) {
      return false;
    }
    if( !Swapchain.Handle ) {
      return true;
    }

    for( size_t i = 0; i < Swapchain.Images.size(); ++i ) {
      Swapchain.ImageViews.emplace_back( LogicalDevice );
      if( !CreateImageView( *LogicalDevice, Swapchain.Images[i], VK_IMAGE_VIEW_TYPE_2D, Swapchain.Format, VK_IMAGE_ASPECT_COLOR_BIT, *Swapchain.ImageViews.back() ) ) {
        return false;
      }
      Swapchain.ImageViewsRaw.push_back( *Swapchain.ImageViews.back() );
    }

    // When we want to use depth buffering, we need to use a depth attachment
    // It must have the same size as the swapchain, so we need to recreate it along with the swapchain
    DepthImages.clear();
    DepthImagesMemory.clear();

    if( use_depth ) {
      for( uint32_t i = 0; i < FramesCount; ++i ) {
        DepthImages.emplace_back( LogicalDevice );
        DepthImagesMemory.emplace_back( LogicalDevice );
        InitVkDestroyer( LogicalDevice, FrameResources[i].DepthAttachment );

        if( !Create2DImageAndView( PhysicalDevice, *LogicalDevice, DepthFormat, Swapchain.Size, 1, 1, VK_SAMPLE_COUNT_1_BIT,
          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, *DepthImages.back(), *DepthImagesMemory.back(),
          *FrameResources[i].DepthAttachment ) ) {
          return false;
        }
      }
    }

    Ready = true;
    return true;
  }

  void VulkanCookbookSample::Deinitialize() {
    if( LogicalDevice ) {
      WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );
    }
  }

} // namespace VulkanCookbook
