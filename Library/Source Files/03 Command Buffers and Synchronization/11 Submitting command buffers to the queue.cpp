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
// Chapter: 03 Command Buffers and Synchronization
// Recipe:  11 Submitting command buffers to the queue

#include "03 Command Buffers and Synchronization/11 Submitting command buffers to the queue.h"

namespace VulkanCookbook {

  bool SubmitCommandBuffersToQueue( VkQueue                         queue,
                                    std::vector<WaitSemaphoreInfo>  wait_semaphore_infos,
                                    std::vector<VkCommandBuffer>    command_buffers,
                                    std::vector<VkSemaphore>        signal_semaphores,
                                    VkFence                         fence ) {
    std::vector<VkSemaphore>          wait_semaphore_handles;
    std::vector<VkPipelineStageFlags> wait_semaphore_stages;

    for( auto & wait_semaphore_info : wait_semaphore_infos ) {
      wait_semaphore_handles.emplace_back( wait_semaphore_info.Semaphore );
      wait_semaphore_stages.emplace_back( wait_semaphore_info.WaitingStage );
    }

    VkSubmitInfo submit_info = {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,                        // VkStructureType                sType
      nullptr,                                              // const void                   * pNext
      static_cast<uint32_t>(wait_semaphore_infos.size()),   // uint32_t                       waitSemaphoreCount
      wait_semaphore_handles.data(),                        // const VkSemaphore            * pWaitSemaphores
      wait_semaphore_stages.data(),                         // const VkPipelineStageFlags   * pWaitDstStageMask
      static_cast<uint32_t>(command_buffers.size()),        // uint32_t                       commandBufferCount
      command_buffers.data(),                               // const VkCommandBuffer        * pCommandBuffers
      static_cast<uint32_t>(signal_semaphores.size()),      // uint32_t                       signalSemaphoreCount
      signal_semaphores.data()                              // const VkSemaphore            * pSignalSemaphores
    };

    VkResult result = vkQueueSubmit( queue, 1, &submit_info, fence );
    if( VK_SUCCESS != result ) {
      std::cout << "Error occurred during command buffer submission." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
