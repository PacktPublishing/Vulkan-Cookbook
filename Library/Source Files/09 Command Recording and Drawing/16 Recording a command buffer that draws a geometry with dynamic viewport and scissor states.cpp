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
// Recipe:  16 Recording a command buffer that draws a geometry with dynamic viewport and scissor states

#include "03 Command Buffers and Synchronization/03 Beginning a command buffer recording operation.h"
#include "03 Command Buffers and Synchronization/04 Ending a command buffer recording operation.h"
#include "04 Resources and Memory/07 Setting an image memory barrier.h"
#include "05 Descriptor Sets/14 Binding descriptor sets.h"
#include "06 Render Passes and Framebuffers/08 Beginning a render pass.h"
#include "06 Render Passes and Framebuffers/10 Ending a render pass.h"
#include "08 Graphics and Compute Pipelines/19 Binding a pipeline object.h"
#include "09 Command Recording and Drawing/07 Setting viewport state dynamically.h"
#include "09 Command Recording and Drawing/08 Setting scissor state dynamically.h"
#include "09 Command Recording and Drawing/12 Drawing a geometry.h"
#include "09 Command Recording and Drawing/16 Recording a command buffer that draws a geometry with dynamic viewport and scissor states.h"

namespace VulkanCookbook {

  bool RecordCommandBufferThatDrawsGeometryWithDynamicViewportAndScissorStates( VkCommandBuffer                             command_buffer,
                                                                                VkImage                                     swapchain_image,
                                                                                uint32_t                                    present_queue_family_index,
                                                                                uint32_t                                    graphics_queue_family_index,
                                                                                VkRenderPass                                render_pass,
                                                                                VkFramebuffer                               framebuffer,
                                                                                VkExtent2D                                  framebuffer_size,
                                                                                std::vector<VkClearValue> const &           clear_values,
                                                                                VkPipeline                                  graphics_pipeline,
                                                                                uint32_t                                    first_vertex_buffer_binding,
                                                                                std::vector<VertexBufferParameters> const & vertex_buffers_parameters,
                                                                                VkPipelineLayout                            pipeline_layout,
                                                                                std::vector<VkDescriptorSet> const &        descriptor_sets,
                                                                                uint32_t                                    index_for_first_descriptor_set,
                                                                                Mesh const &                                geometry,
                                                                                uint32_t                                    instance_count,
                                                                                uint32_t                                    first_instance ) {
    if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    if( present_queue_family_index != graphics_queue_family_index ) {
      ImageTransition image_transition_before_drawing = {
        swapchain_image,                          // VkImage              Image
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
        present_queue_family_index,               // uint32_t             CurrentQueueFamily
        graphics_queue_family_index,              // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
      };
      SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
    }

    BeginRenderPass( command_buffer, render_pass, framebuffer, { { 0, 0 }, framebuffer_size }, clear_values, VK_SUBPASS_CONTENTS_INLINE );

    BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline );

    VkViewport viewport = {
      0.0f,                                         // float          x
      0.0f,                                         // float          y
      static_cast<float>(framebuffer_size.width),   // float          width
      static_cast<float>(framebuffer_size.height),  // float          height
      0.0f,                                         // float          minDepth
      1.0f,                                         // float          maxDepth
    };
    SetViewportStateDynamically( command_buffer, 0, { viewport } );

    VkRect2D scissor = {
      {                                             // VkOffset2D     offset
        0,                                            // int32_t        x
        0                                             // int32_t        y
      },
      {                                             // VkExtent2D     extent
        framebuffer_size.width,                         // uint32_t       width
        framebuffer_size.height                         // uint32_t       height
      }
    };
    SetScissorStateDynamically( command_buffer, 0, { scissor } );

    BindVertexBuffers( command_buffer, first_vertex_buffer_binding, vertex_buffers_parameters );

    if( descriptor_sets.size() > 0 ) {
      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, index_for_first_descriptor_set, descriptor_sets, {} );
    }

    for( size_t i = 0; i < geometry.Parts.size(); ++i ) {
      DrawGeometry( command_buffer, geometry.Parts[i].VertexCount, instance_count, geometry.Parts[i].VertexOffset, first_instance );
    }

    EndRenderPass( command_buffer );

    if( present_queue_family_index != graphics_queue_family_index ) {
      ImageTransition image_transition_before_present = {
        swapchain_image,                          // VkImage              Image
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        CurrentAccess
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
        graphics_queue_family_index,              // uint32_t             CurrentQueueFamily
        present_queue_family_index,               // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
      };
      SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
    }

    if( !EndCommandBufferRecordingOperation( command_buffer ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
