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
// Recipe:  17 Recording command buffers on multiple threads

#ifndef RECORDING_COMMAND_BUFFERS_ON_MULTIPLE_THREADS
#define RECORDING_COMMAND_BUFFERS_ON_MULTIPLE_THREADS

#include "03 Command Buffers and Synchronization/11 Submitting command buffers to the queue.h"
#include "Common.h"

namespace VulkanCookbook {

  struct CommandBufferRecordingThreadParameters {
    VkCommandBuffer                         CommandBuffer;
    std::function<bool( VkCommandBuffer )>  RecordingFunction;
  };

  bool RecordCommandBuffersOnMultipleThreads( std::vector<CommandBufferRecordingThreadParameters> const & threads_parameters,
                                              VkQueue                                                     queue,
                                              std::vector<WaitSemaphoreInfo>                              wait_semaphore_infos,
                                              std::vector<VkSemaphore>                                    signal_semaphores,
                                              VkFence                                                     fence );

} // namespace VulkanCookbook

#endif // RECORDING_COMMAND_BUFFERS_ON_MULTIPLE_THREADS