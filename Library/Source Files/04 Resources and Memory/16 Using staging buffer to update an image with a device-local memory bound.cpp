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
// Chapter: 04 Resources and Memory
// Recipe:  16 Using staging buffer to update an image with a device-local memory bound

#include "03 Command Buffers and Synchronization/03 Beginning a command buffer recording operation.h"
#include "03 Command Buffers and Synchronization/04 Ending a command buffer recording operation.h"
#include "03 Command Buffers and Synchronization/08 Creating a fence.h"
#include "03 Command Buffers and Synchronization/09 Waiting for fences.h"
#include "03 Command Buffers and Synchronization/11 Submitting command buffers to the queue.h"
#include "04 Resources and Memory/01 Creating a buffer.h"
#include "04 Resources and Memory/02 Allocating and binding memory object to a buffer.h"
#include "04 Resources and Memory/07 Setting an image memory barrier.h"
#include "04 Resources and Memory/11 Mapping, updating and unmapping host-visible memory.h"
#include "04 Resources and Memory/13 Copying data from a buffer to an image.h"
#include "04 Resources and Memory/16 Using staging buffer to update an image with a device-local memory bound.h"
#include "04 Resources and Memory/20 Freeing a memory object.h"
#include "04 Resources and Memory/21 Destroying a buffer.h"

namespace VulkanCookbook {

  bool UseStagingBufferToUpdateImageWithDeviceLocalMemoryBound( VkPhysicalDevice           physical_device,
                                                                VkDevice                   logical_device,
                                                                VkDeviceSize               data_size,
                                                                void                     * data,
                                                                VkImage                    destination_image,
                                                                VkImageSubresourceLayers   destination_image_subresource,
                                                                VkOffset3D                 destination_image_offset,
                                                                VkExtent3D                 destination_image_size,
                                                                VkImageLayout              destination_image_current_layout,
                                                                VkImageLayout              destination_image_new_layout,
                                                                VkAccessFlags              destination_image_current_access,
                                                                VkAccessFlags              destination_image_new_access,
                                                                VkImageAspectFlags         destination_image_aspect,
                                                                VkPipelineStageFlags       destination_image_generating_stages,
                                                                VkPipelineStageFlags       destination_image_consuming_stages,
                                                                VkQueue                    queue,
                                                                VkCommandBuffer            command_buffer,
                                                                std::vector<VkSemaphore>   signal_semaphores ) {

    VkDestroyer<VkBuffer> staging_buffer;
    InitVkDestroyer( logical_device, staging_buffer );
    if( !CreateBuffer( logical_device, data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, *staging_buffer ) ) {
      return false;
    }

    VkDestroyer<VkDeviceMemory> memory_object;
    InitVkDestroyer( logical_device, memory_object );
    if( !AllocateAndBindMemoryObjectToBuffer( physical_device, logical_device, *staging_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, *memory_object ) ) {
      return false;
    }

    if( !MapUpdateAndUnmapHostVisibleMemory( logical_device, *memory_object, 0, data_size, data, true, nullptr ) ) {
      return false;
    }

    if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    SetImageMemoryBarrier( command_buffer, destination_image_generating_stages, VK_PIPELINE_STAGE_TRANSFER_BIT,
    {
      {
        destination_image,                        // VkImage            Image
        destination_image_current_access,         // VkAccessFlags      CurrentAccess
        VK_ACCESS_TRANSFER_WRITE_BIT,             // VkAccessFlags      NewAccess
        destination_image_current_layout,         // VkImageLayout      CurrentLayout
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,     // VkImageLayout      NewLayout
        VK_QUEUE_FAMILY_IGNORED,                  // uint32_t           CurrentQueueFamily
        VK_QUEUE_FAMILY_IGNORED,                  // uint32_t           NewQueueFamily
        destination_image_aspect                  // VkImageAspectFlags Aspect
      } } );

    CopyDataFromBufferToImage( command_buffer, *staging_buffer, destination_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    {
      {
        0,                                        // VkDeviceSize               bufferOffset
        0,                                        // uint32_t                   bufferRowLength
        0,                                        // uint32_t                   bufferImageHeight
        destination_image_subresource,            // VkImageSubresourceLayers   imageSubresource
        destination_image_offset,                 // VkOffset3D                 imageOffset
        destination_image_size,                   // VkExtent3D                 imageExtent
      } } );

    SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, destination_image_consuming_stages,
    {
      {
        destination_image,                        // VkImage            Image
        VK_ACCESS_TRANSFER_WRITE_BIT,             // VkAccessFlags      CurrentAccess
        destination_image_new_access,             // VkAccessFlags      NewAccess
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,     // VkImageLayout      CurrentLayout
        destination_image_new_layout,             // VkImageLayout      NewLayout
        VK_QUEUE_FAMILY_IGNORED,                  // uint32_t           CurrentQueueFamily
        VK_QUEUE_FAMILY_IGNORED,                  // uint32_t           NewQueueFamily
        destination_image_aspect                  // VkImageAspectFlags Aspect
      } } );

    if( !EndCommandBufferRecordingOperation( command_buffer ) ) {
      return false;
    }

    VkDestroyer<VkFence> fence;
    InitVkDestroyer( logical_device, fence );
    if( !CreateFence( logical_device, false, *fence ) ) {
      return false;
    }

    if( !SubmitCommandBuffersToQueue( queue, {}, { command_buffer }, signal_semaphores, *fence ) ) {
      return false;
    }

    if( !WaitForFences( logical_device, { *fence }, VK_FALSE, 500000000 ) ) {
      return false;
    }

    return true;
  }

} // namespace VulkanCookbook
