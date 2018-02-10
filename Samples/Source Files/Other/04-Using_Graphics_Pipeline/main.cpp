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
// 04-Using_Graphics_Pipeline

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  VkDestroyer(VkCommandPool)    CommandPool;
  VkCommandBuffer               CommandBuffer;
  VkDestroyer(VkRenderPass)     RenderPass;
  VkDestroyer(VkFramebuffer)    Framebuffer;
  VkDestroyer(VkPipeline)       GraphicsPipeline;
  VkDestroyer(VkPipelineLayout) PipelineLayout;
  VkDestroyer(VkBuffer)         VertexBuffer;
  VkDestroyer(VkDeviceMemory)   BufferMemory;
  VkDestroyer(VkFence)          DrawingFence;
  VkDestroyer(VkSemaphore)      ImageAcquiredSemaphore;
  VkDestroyer(VkSemaphore)      ReadyToPresentSemaphore;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    if( !InitializeVulkan( window_parameters ) ) {
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

    // Graphics pipeline
    std::vector<unsigned char> vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/04 Using Graphics Pipeline/shader.vert.spv", vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer(VkShaderModule) vertex_shader_module;
    InitVkDestroyer( LogicalDevice, vertex_shader_module );
    if( !CreateShaderModule( *LogicalDevice, vertex_shader_spirv, *vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/04 Using Graphics Pipeline/shader.frag.spv", fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer(VkShaderModule) fragment_shader_module;
    InitVkDestroyer( LogicalDevice, fragment_shader_module );
    if( !CreateShaderModule( *LogicalDevice, fragment_shader_spirv, *fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,   // VkShaderStageFlagBits        ShaderStage
        *vertex_shader_module,        // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName
        nullptr                       // VkSpecializationInfo const * SpecializationInfo
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT, // VkShaderStageFlagBits        ShaderStage
        *fragment_shader_module,      // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName
        nullptr                       // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        3 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> vertex_attribute_descriptions = {
      {
        0,                            // uint32_t                     location
        0,                            // uint32_t                     binding
        VK_FORMAT_R32G32B32_SFLOAT,   // VkFormat                     format
        0                             // uint32_t                     offset
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( vertex_input_binding_descriptions, vertex_attribute_descriptions, vertex_input_state_create_info );

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, false, input_assembly_state_create_info );

    ViewportInfo viewport_infos = {
      {                     // std::vector<VkViewport>   Viewports
        {
          0.0f,               // float          x
          0.0f,               // float          y
          500.0f,             // float          width
          500.0f,             // float          height
          0.0f,               // float          minDepth
          1.0f                // float          maxDepth
        }
      },
      {                     // std::vector<VkRect2D>     Scissors;
        {
          {                   // VkOffset2D     offset
            0,                  // int32_t        x
            0                   // int32_t        y
          },
          {                   // VkExtent2D     extent
            500,                // uint32_t       width
            500                 // uint32_t       height
          }
        }
      }
    };
    VkPipelineViewportStateCreateInfo viewport_state_create_info;
    SpecifyPipelineViewportAndScissorTestState( viewport_infos, viewport_state_create_info );

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 0.0f, 0.0f, 1.0f, rasterization_state_create_info );

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    std::vector<VkPipelineColorBlendAttachmentState> attachment_blend_states = {
      {
        false,                          // VkBool32                 blendEnable
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcColorBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstColorBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                colorBlendOp
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcAlphaBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT |      // VkColorComponentFlags    colorWriteMask
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT
      }
    };
    VkPipelineColorBlendStateCreateInfo blend_state_create_info;
    SpecifyPipelineBlendState( false, VK_LOGIC_OP_COPY, attachment_blend_states, { 1.0f, 1.0f, 1.0f, 1.0f }, blend_state_create_info );

    std::vector<VkDynamicState> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info;
    SpecifyPipelineDynamicStates( dynamic_states, dynamic_state_create_info );

    InitVkDestroyer( LogicalDevice, PipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, {}, {}, *PipelineLayout ) ) {
      return false;
    }

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, nullptr, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, graphics_pipeline_create_info );

    std::vector<VkPipeline> graphics_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { graphics_pipeline_create_info }, VK_NULL_HANDLE, graphics_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, GraphicsPipeline );
    *GraphicsPipeline = graphics_pipeline[0];

    // Vertex data
    std::vector<float> vertices = {
      0.0f, -0.75f, 0.0f,
      -0.75f, 0.75f, 0.0f,
      0.75f, 0.75f, 0.0f
    };

    InitVkDestroyer( LogicalDevice, VertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *VertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, BufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *VertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *BufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( vertices[0] ) * vertices.size(), &vertices[0], *VertexBuffer, 0, 0,
      VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, GraphicsQueue.Handle, CommandBuffer, {} ) ) {
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
        Swapchain.Images[image_index],            // VkImage              Image
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
        PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
        GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
      };
      SetImageMemoryBarrier( CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
    }

    // Drawing
    BeginRenderPass( CommandBuffer, *RenderPass, *Framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f } }, VK_SUBPASS_CONTENTS_INLINE );

    BindPipelineObject( CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *GraphicsPipeline );
    VkViewport viewport = {
      0.0f,                                       // float    x
      0.0f,                                       // float    y
      static_cast<float>(Swapchain.Size.width),   // float    width
      static_cast<float>(Swapchain.Size.height),  // float    height
      0.0f,                                       // float    minDepth
      1.0f,                                       // float    maxDepth
    };
    SetViewportStateDynamically( CommandBuffer, 0, { viewport } );

    VkRect2D scissor = {
      {                                         // VkOffset2D     offset
        0,                                        // int32_t        x
        0                                         // int32_t        y
      },
      {                                         // VkExtent2D     extent
        Swapchain.Size.width,                     // uint32_t       width
        Swapchain.Size.height                     // uint32_t       height
      }
    };
    SetScissorStateDynamically( CommandBuffer, 0, { scissor } );

    BindVertexBuffers( CommandBuffer, 0, { { *VertexBuffer, 0 } } );

    DrawGeometry( CommandBuffer, 3, 1, 0, 0 );

    EndRenderPass( CommandBuffer );

    if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
      ImageTransition image_transition_before_present = {
        Swapchain.Images[image_index],            // VkImage              Image
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
        VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
        GraphicsQueue.FamilyIndex,                // uint32_t             CurrentQueueFamily
        PresentQueue.FamilyIndex,                 // uint32_t             NewQueueFamily
        VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
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
      *Swapchain.Handle,    // VkSwapchainKHR   Swapchain
      image_index           // uint32_t         ImageIndex
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

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "04 - Using Graphics Pipeline", 50, 25, 1280, 800, Sample )
