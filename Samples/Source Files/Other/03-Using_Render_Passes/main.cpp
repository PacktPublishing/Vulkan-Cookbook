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
// 03-Using_Render_Passes

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  VkDestroyer<VkCommandPool>  CommandPool;
  VkCommandBuffer             CommandBuffer;
  VkDestroyer<VkRenderPass>   RenderPass;
  VkDestroyer<VkFramebuffer>  Framebuffer;
  VkDestroyer<VkFence>        DrawingFence;
  VkDestroyer<VkSemaphore>    ImageAcquiredSemaphore;
  VkDestroyer<VkSemaphore>    ReadyToPresentSemaphore;

  virtual bool Initialize( WindowParameters WindowParameters ) override {
    if( !InitializeVulkan( WindowParameters ) ) {
      return false;
    }

    // Command buffers creation
    InitVkDestroyer( LogicalDevice, CommandPool );
    if( !CreateCommandPool( *LogicalDevice, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, GraphicsQueue.FamilyIndex, *CommandPool ) ) {
      return false;
    }

    std::vector<VkCommandBuffer> command_buffers;
    if( !AllocateCommandBuffers( *LogicalDevice, *CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, command_buffers ) ) {
      return false;
    }
    CommandBuffer = command_buffers[0];

    // Drawing synchronization
    InitVkDestroyer( LogicalDevice, DrawingFence );
    if( !CreateFence( *LogicalDevice, true, *DrawingFence ) ) {
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

    // Render pass
    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE, // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR   // VkImageLayout                    finalLayout
      }
    };

    std::vector<SubpassParameters> subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,            // VkPipelineBindPoint                  PipelineType
        {},                                         // std::vector<VkAttachmentReference>   InputAttachments
        {
          {                                         // std::vector<VkAttachmentReference>   ColorAttachments
            0,                                        // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout                        layout
          }
        },
        {},                                         // std::vector<VkAttachmentReference>   ResolveAttachments
        nullptr,                                    // VkAttachmentReference const        * DepthStencilAttachment
        {}                                          // std::vector<uint32_t>                PreserveAttachments
      }
    };

    std::vector<VkSubpassDependency> subpass_dependencies = {
      {
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
        0,                                              // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              srcAccessMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      },
      {
        0,                                              // uint32_t                   srcSubpass
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              srcAccessMask
        VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      }
    };

    InitVkDestroyer( LogicalDevice, RenderPass );
    if( !CreateRenderPass( *LogicalDevice, attachment_descriptions, subpass_parameters, subpass_dependencies, *RenderPass ) ) {
      return false;
    }

    return true;
  }

  virtual bool Draw() override {
    if( !WaitForFences( *LogicalDevice, { *DrawingFence }, false, 5000000000 ) ) {
      return false;
    }

    if( !ResetFences( *LogicalDevice, { *DrawingFence } ) ) {
      return false;
    }

    uint32_t image_index;
    if( !AcquireSwapchainImage( *LogicalDevice, *Swapchain.Handle, *ImageAcquiredSemaphore, VK_NULL_HANDLE, image_index ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, Framebuffer );
    if( !CreateFramebuffer( *LogicalDevice, *RenderPass, { *Swapchain.ImageViews[image_index] }, Swapchain.Size.width, Swapchain.Size.height, 1, *Framebuffer ) ) {
      return false;
    }

    if( !BeginCommandBufferRecordingOperation( CommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
      ImageTransition image_transition_before_drawing = {
        Swapchain.Images[image_index],              // VkImage              Image
        VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags        CurrentAccess
        VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout        NewLayout
        PresentQueue.FamilyIndex,                   // uint32_t             CurrentQueueFamily
        GraphicsQueue.FamilyIndex,                  // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                   // VkImageAspectFlags   Aspect
      };
      SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
    }

    // Drawing
    BeginRenderPass( CommandBuffer, *RenderPass, *Framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.2f, 0.5f, 0.8f, 1.0f } }, VK_SUBPASS_CONTENTS_INLINE );

    EndRenderPass( CommandBuffer );

    if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
      ImageTransition image_transition_before_present = {
        Swapchain.Images[image_index],              // VkImage              Image
        VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags        CurrentAccess
        VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout        NewLayout
        GraphicsQueue.FamilyIndex,                  // uint32_t             CurrentQueueFamily
        PresentQueue.FamilyIndex,                   // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                   // VkImageAspectFlags   Aspect
      };
      SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
    }

    if( !EndCommandBufferRecordingOperation( CommandBuffer ) ) {
      return false;
    }

    WaitSemaphoreInfo wait_semaphore_info = {
      *ImageAcquiredSemaphore,            // VkSemaphore            Semaphore
      VK_PIPELINE_STAGE_ALL_COMMANDS_BIT  // VkPipelineStageFlags   WaitingStage
    };
    if( !SubmitCommandBuffersToQueue( GraphicsQueue.Handle, { wait_semaphore_info }, { CommandBuffer }, { *ReadyToPresentSemaphore }, *DrawingFence ) ) {
      return false;
    }

    PresentInfo present_info = {
      *Swapchain.Handle,  // VkSwapchainKHR   Swapchain
      image_index         // uint32_t         ImageIndex
    };
    if( !PresentImage( PresentQueue.Handle, { *ReadyToPresentSemaphore }, { present_info } ) ) {
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

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "03 - Using Render Passes", 50, 25, 1280, 800, Sample )
