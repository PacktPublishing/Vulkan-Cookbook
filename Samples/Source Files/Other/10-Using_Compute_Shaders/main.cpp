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
// 10-Using_Compute_Shaders

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  VkDestroyer(VkCommandPool)          CommandPool;
  VkCommandBuffer                     CommandBuffer;
  VkDestroyer(VkImage)                Image;
  VkDestroyer(VkDeviceMemory)         ImageMemory;
  VkDestroyer(VkImageView)            ImageView;
  VkDestroyer(VkDescriptorSetLayout)  DescriptorSetLayout;
  VkDestroyer(VkDescriptorPool)       DescriptorPool;
  std::vector<VkDescriptorSet>        DescriptorSets;
  VkDestroyer(VkFence)                DrawingFence;
  VkDestroyer(VkSemaphore)            ImageAcquiredSemaphore;
  VkDestroyer(VkSemaphore)            ReadyToPresentSemaphore;
  VkDestroyer(VkFramebuffer)          Framebuffer;
  VkDestroyer(VkPipeline)             ComputePipeline;
  VkDestroyer(VkPipelineLayout)       PipelineLayout;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    if( !InitializeVulkan( window_parameters, nullptr, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, false ) ) {
      return false;
    }

    // Command buffers creation
    InitVkDestroyer( LogicalDevice, CommandPool );
    if( !CreateCommandPool( *LogicalDevice, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, ComputeQueue.FamilyIndex, *CommandPool ) ) {
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

    // Storage Image
    InitVkDestroyer( LogicalDevice, Image );
    InitVkDestroyer( LogicalDevice, ImageMemory );
    InitVkDestroyer( LogicalDevice, ImageView );
    if( !Create2DImageAndView( PhysicalDevice, *LogicalDevice, Swapchain.Format, Swapchain.Size, 1, 1, VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_ASPECT_COLOR_BIT, *Image, *ImageMemory, *ImageView ) ) {
      return false;
    }

    // Descriptor set with storage image
    VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
      0,                                // uint32_t             binding
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, // VkDescriptorType     descriptorType
      1,                                // uint32_t             descriptorCount
      VK_SHADER_STAGE_COMPUTE_BIT,      // VkShaderStageFlags   stageFlags
      nullptr                           // const VkSampler    * pImmutableSamplers
    };
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout );
    if( !CreateDescriptorSetLayout( *LogicalDevice, { descriptor_set_layout_binding }, *DescriptorSetLayout ) ) {
      return false;
    }

    VkDescriptorPoolSize descriptor_pool_size = {
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, // VkDescriptorType     type
      1                                 // uint32_t             descriptorCount
    };
    InitVkDestroyer( LogicalDevice, DescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 1, { descriptor_pool_size }, *DescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *DescriptorPool, { *DescriptorSetLayout }, DescriptorSets ) ) {
      return false;
    }

    ImageDescriptorInfo image_descriptor_update = {
      DescriptorSets[0],                // VkDescriptorSet                      TargetDescriptorSet
      0,                                // uint32_t                             TargetDescriptorBinding
      0,                                // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, // VkDescriptorType                     TargetDescriptorType
      {                                 // std::vector<VkDescriptorImageInfo>   ImageInfos
        {
          VK_NULL_HANDLE,                 // VkSampler                            sampler
          *ImageView,                     // VkImageView                          imageView
          VK_IMAGE_LAYOUT_GENERAL         // VkImageLayout                        imageLayout
        }
      }
    };

    UpdateDescriptorSets( *LogicalDevice, { image_descriptor_update }, {}, {}, {} );

    // Compute pipeline
    std::vector<unsigned char> compute_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/10 Using Compute Shaders/shader.comp.spv", compute_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) compute_shader_module;
    InitVkDestroyer( LogicalDevice, compute_shader_module );
    if( !CreateShaderModule( *LogicalDevice, compute_shader_spirv, *compute_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shader_stage_params = {
      {
        VK_SHADER_STAGE_COMPUTE_BIT,      // VkShaderStageFlagBits        ShaderStage
        *compute_shader_module,           // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    InitVkDestroyer( LogicalDevice, PipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout }, {}, *PipelineLayout ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ComputePipeline );
    if( !CreateComputePipeline( *LogicalDevice, 0, shader_stage_create_infos[0], *PipelineLayout, VK_NULL_HANDLE, VK_NULL_HANDLE, *ComputePipeline ) ) {
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

    if( !BeginCommandBufferRecordingOperation( CommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    uint32_t present_queue_family_index = PresentQueue.FamilyIndex;
    uint32_t compute_queue_family_index = ComputeQueue.FamilyIndex;
    if( PresentQueue.FamilyIndex == ComputeQueue.FamilyIndex ) {
      present_queue_family_index = VK_QUEUE_FAMILY_IGNORED;
      compute_queue_family_index = VK_QUEUE_FAMILY_IGNORED;
    }

    ImageTransition image_transition_for_compute_shader = {
      *Image,                                   // VkImage              Image
      0,                                        // VkAccessFlags        CurrentAccess
      VK_ACCESS_SHADER_WRITE_BIT,               // VkAccessFlags        NewAccess
      VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
      VK_IMAGE_LAYOUT_GENERAL,                  // VkImageLayout        NewLayout
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             CurrentQueueFamily
      VK_QUEUE_FAMILY_IGNORED,                  // uint32_t             NewQueueFamily
      VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
    };
    SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, { image_transition_for_compute_shader } );

    // Dispatching compute work
    BindDescriptorSets( CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, *PipelineLayout, 0, DescriptorSets, {} );

    BindPipelineObject( CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, *ComputePipeline );

    DispatchComputeWork( CommandBuffer, Swapchain.Size.width / 32 + 1, Swapchain.Size.height / 32 + 1, 1 );

    std::vector<ImageTransition> image_transitions_for_transfer = {
      {
        *Image,                                 // VkImage              Image
        VK_ACCESS_SHADER_WRITE_BIT,             // VkAccessFlags        CurrentAccess
        VK_ACCESS_TRANSFER_READ_BIT,            // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_GENERAL,                // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,   // VkImageLayout        NewLayout
        VK_QUEUE_FAMILY_IGNORED,                // uint32_t             CurrentQueueFamily
        VK_QUEUE_FAMILY_IGNORED,                // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT               // VkImageAspectFlags   Aspect
      },
      {
        Swapchain.Images[image_index],          // VkImage              Image
        0,                                      // VkAccessFlags        CurrentAccess
        VK_ACCESS_TRANSFER_WRITE_BIT,           // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_UNDEFINED,              // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,   // VkImageLayout        NewLayout
        present_queue_family_index,             // uint32_t             CurrentQueueFamily
        compute_queue_family_index,             // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT               // VkImageAspectFlags   Aspect
      },
    };
    SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, image_transitions_for_transfer );

    VkImageCopy image_copy = {
      {                             // VkImageSubresourceLayers     srcSubresource
        VK_IMAGE_ASPECT_COLOR_BIT,    // VkImageAspectFlags           aspectMask
        0,                            // uint32_t                     mipLevel
        0,                            // uint32_t                     baseArrayLayer
        1                             // uint32_t                     layerCount
      },
      {                             // VkOffset3D                   srcOffset
        0,                            // int32_t                      x
        0,                            // int32_t                      y
        0                             // int32_t                      z
      },
      {                             // VkImageSubresourceLayers     dstSubresource
        VK_IMAGE_ASPECT_COLOR_BIT,    // VkImageAspectFlags           aspectMask
        0,                            // uint32_t                     mipLevel
        0,                            // uint32_t                     baseArrayLayer
        1                             // uint32_t                     layerCount
      },
      {                             // VkOffset3D                   dstOffset
        0,                            // int32_t                      x
        0,                            // int32_t                      y
        0                             // int32_t                      z
      },
      {                             // VkExtent3D                   extent
        Swapchain.Size.width,         // uint32_t                     width
        Swapchain.Size.height,        // uint32_t                     height
        1                             // uint32_t                     depth
      }
    };
    vkCmdCopyImage( CommandBuffer, *Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, Swapchain.Images[image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &image_copy );

    ImageTransition image_transition_before_present = {
      Swapchain.Images[image_index],             // VkImage              Image
      VK_ACCESS_TRANSFER_WRITE_BIT,             // VkAccessFlags        CurrentAccess
      VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,     // VkImageLayout        CurrentLayout
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
      compute_queue_family_index,               // uint32_t             CurrentQueueFamily
      present_queue_family_index,               // uint32_t             NewQueueFamily
      VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
    };
    SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );

    if( !EndCommandBufferRecordingOperation( CommandBuffer ) ) {
      return false;
    }

    WaitSemaphoreInfo wait_semaphore_info = {
      *ImageAcquiredSemaphore,            // VkSemaphore            Semaphore
      VK_PIPELINE_STAGE_ALL_COMMANDS_BIT  // VkPipelineStageFlags   WaitingStage
    };
    if( !SubmitCommandBuffersToQueue( ComputeQueue.Handle, { wait_semaphore_info }, { CommandBuffer }, { *ReadyToPresentSemaphore }, *DrawingFence ) ) {
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
    if( !CreateSwapchain( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, false ) ) {
      return false;
    }
    // Storage Image
    DestroyImageView( *LogicalDevice, *ImageView );

    FreeMemoryObject( *LogicalDevice, *ImageMemory );

    DestroyImage( *LogicalDevice, *Image );

    if( !Create2DImageAndView( PhysicalDevice, *LogicalDevice, Swapchain.Format, Swapchain.Size, 1, 1, VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_ASPECT_COLOR_BIT, *Image, *ImageMemory, *ImageView ) ) {
      return false;
    }

    ImageDescriptorInfo image_descriptor_update = {
      DescriptorSets[0],                  // VkDescriptorSet                      TargetDescriptorSet
      0,                                  // uint32_t                             TargetDescriptorBinding
      0,                                  // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,   // VkDescriptorType                     TargetDescriptorType
      {                                   // std::vector<VkDescriptorImageInfo>   ImageInfos
        {
          VK_NULL_HANDLE,                   // VkSampler                            sampler
          *ImageView,                       // VkImageView                          imageView
          VK_IMAGE_LAYOUT_GENERAL           // VkImageLayout                        imageLayout
        }
      }
    };

    UpdateDescriptorSets( *LogicalDevice, { image_descriptor_update }, {}, {}, {} );
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "10 - Using Compute Shaders", 50, 25, 1280, 800, Sample )
