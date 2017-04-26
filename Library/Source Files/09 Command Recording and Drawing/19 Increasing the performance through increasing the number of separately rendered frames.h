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

#ifndef INCREASING_THE_PERFORMANCE_THROUGH_INCREASING_THE_NUMBER_OF_SEPARATELY_RENDERED_FRAMES
#define INCREASING_THE_PERFORMANCE_THROUGH_INCREASING_THE_NUMBER_OF_SEPARATELY_RENDERED_FRAMES

#include "03 Command Buffers and Synchronization/11 Submitting command buffers to the queue.h"
#include "Common.h"

namespace VulkanCookbook {

  struct FrameResources {
    VkCommandBuffer             CommandBuffer;
    VkDestroyer<VkSemaphore>    ImageAcquiredSemaphore;
    VkDestroyer<VkSemaphore>    ReadyToPresentSemaphore;
    VkDestroyer<VkFence>        DrawingFinishedFence;
    VkDestroyer<VkImageView>    DepthAttachment;
    VkDestroyer<VkFramebuffer>  Framebuffer;
  };

  bool IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( VkDevice                                                        logical_device,
                                                                                VkQueue                                                         graphics_queue,
                                                                                VkQueue                                                         present_queue,
                                                                                VkSwapchainKHR                                                  swapchain,
                                                                                VkExtent2D                                                      swapchain_size,
                                                                                std::vector<VkImageView> const                                & swapchain_image_views,
                                                                                VkRenderPass                                                    render_pass,
                                                                                std::vector<WaitSemaphoreInfo> const                          & wait_infos,
                                                                                std::function<bool(VkCommandBuffer, uint32_t, VkFramebuffer)>   record_command_buffer,
                                                                                std::vector<FrameResources>                                   & frame_resources );

} // namespace VulkanCookbook

#endif // INCREASING_THE_PERFORMANCE_THROUGH_INCREASING_THE_NUMBER_OF_SEPARATELY_RENDERED_FRAMES