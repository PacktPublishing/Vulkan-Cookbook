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
// Chapter: 09 Command Recording and Drawing
// Recipe:  19 Increasing the performance through increasing the number of separately rendered frames

#include "03 Command Buffers and Synchronization/09 Waiting for fences.h"
#include "03 Command Buffers and Synchronization/10 Resetting fences.h"
#include "06 Render Passes and Framebuffers/11 Destroying a framebuffer.h"
#include "09 Command Recording and Drawing/18 Preparing a single frame of animation.h"
#include "09 Command Recording and Drawing/19 Increasing the performance through increasing the number of separately rendered frames.h"

namespace VulkanCookbook {

  bool IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( VkDevice                                                        logical_device,
                                                                                VkQueue                                                         graphics_queue,
                                                                                VkQueue                                                         present_queue,
                                                                                VkSwapchainKHR                                                  swapchain,
                                                                                VkExtent2D                                                      swapchain_size,
                                                                                std::vector<VkImageView> const                                & swapchain_image_views,
                                                                                VkRenderPass                                                    render_pass,
                                                                                std::vector<WaitSemaphoreInfo> const                          & wait_infos,
                                                                                std::function<bool(VkCommandBuffer, uint32_t, VkFramebuffer)>   record_command_buffer,
                                                                                std::vector<FrameResources>                                   & frame_resources ) {
    static uint32_t frame_index = 0;
    FrameResources & current_frame = frame_resources[frame_index];

    if( !WaitForFences( logical_device, { *current_frame.DrawingFinishedFence }, false, 2000000000 ) ) {
      return false;
    }
    if( !ResetFences( logical_device, { *current_frame.DrawingFinishedFence } ) ) {
      return false;
    }

    InitVkDestroyer( logical_device, current_frame.Framebuffer );

    if( !PrepareSingleFrameOfAnimation( logical_device, graphics_queue, present_queue, swapchain, swapchain_size, swapchain_image_views,
      *current_frame.DepthAttachment, wait_infos, *current_frame.ImageAcquiredSemaphore, *current_frame.ReadyToPresentSemaphore,
      *current_frame.DrawingFinishedFence, record_command_buffer, current_frame.CommandBuffer, render_pass, current_frame.Framebuffer ) ) {
      return false;
    }

    frame_index = (frame_index + 1) % frame_resources.size();
    return true;
  }

} // namespace VulkanCookbook
