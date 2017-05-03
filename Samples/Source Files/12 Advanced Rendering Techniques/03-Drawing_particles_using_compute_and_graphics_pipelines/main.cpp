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
// Chapter: 12 Advanced Rendering Techniques
// Recipe:  03 Drawing particles using compute and graphics pipelines

#include "CookbookSampleFramework.h"
#include "OrbitingCamera.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  VkDestroyer<VkCommandPool>                      ComputeCommandPool;
  VkCommandBuffer                                 ComputeCommandBuffer;
  VkDestroyer<VkSemaphore>                        ComputeSemaphore;
  VkDestroyer<VkFence>                            ComputeFence;

  const uint32_t                                  PARTICLES_COUNT = 2000;
  VkDestroyer<VkBuffer>                           VertexBuffer;
  VkDestroyer<VkDeviceMemory>                     VertexBufferMemory;
  VkDestroyer<VkBufferView>                       VertexBufferView;

  bool                                            UpdateUniformBuffer;
  VkDestroyer<VkBuffer>                           UniformBuffer;
  VkDestroyer<VkDeviceMemory>                     UniformBufferMemory;

  std::vector<VkDestroyer<VkDescriptorSetLayout>> DescriptorSetLayout;
  VkDestroyer<VkDescriptorPool>                   DescriptorPool;
  std::vector<VkDescriptorSet>                    DescriptorSets;

  VkDestroyer<VkPipelineLayout>                   ComputePipelineLayout;
  VkDestroyer<VkPipeline>                         ComputePipeline;

  VkDestroyer<VkRenderPass>                       RenderPass;
  VkDestroyer<VkPipelineLayout>                   GraphicsPipelineLayout;
  VkDestroyer<VkPipeline>                         GraphicsPipeline;

  VkDestroyer<VkBuffer>                           StagingBuffer;
  VkDestroyer<VkDeviceMemory>                     StagingBufferMemory;

  OrbitingCamera                                  Camera;


  static const VkFormat DepthFormat = VK_FORMAT_D16_UNORM;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    VkPhysicalDeviceFeatures device_features = {};
    device_features.geometryShader = true;

    if( !InitializeVulkan( window_parameters, &device_features ) ) {
      return false;
    }

    Camera = OrbitingCamera( Vector3{ 0.0f, 0.0f, 0.0f }, 4.0f );

    // Compute command buffer creation

    InitVkDestroyer( LogicalDevice, ComputeCommandPool );
    if( !CreateCommandPool( *LogicalDevice, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, ComputeQueue.FamilyIndex, *ComputeCommandPool ) ) {
      return false;
    }

    std::vector<VkCommandBuffer> command_buffers;
    if( !AllocateCommandBuffers( *LogicalDevice, *ComputeCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, command_buffers ) ) {
      return false;
    }
    ComputeCommandBuffer = command_buffers[0];

    // Vertex buffer / storage texel buffer

    {
      std::vector<float> particles;

      for( uint32_t i = 0; i < PARTICLES_COUNT; ++i ) {
        OrbitingCamera particle( { 0.0f, 0.0f, 0.0f }, 1.5f, static_cast<float>((std::rand() % 181) - 90), static_cast<float>((std::rand() % 51) - 25) );
        Vector3 position = particle.GetPosition();
        Vector3 color = 0.0075f * Vector3{
          250.0f - std::abs( particle.GetVerticalAngle() * 10.0f ),
          static_cast<float>(std::rand() % 61 + 40),
          static_cast<float>(std::rand() % 61)
        };
        float speed = 0.5f + 0.01f * static_cast<float>(std::rand() % 101) + color[0] * 0.5f;
        particles.insert( particles.end(), position.begin(), position.end() );
        particles.push_back( 1.0f );
        particles.insert( particles.end(), color.begin(), color.end() );
        particles.push_back( speed );
      }

      InitVkDestroyer( LogicalDevice, VertexBuffer );
      InitVkDestroyer( LogicalDevice, VertexBufferMemory );
      InitVkDestroyer( LogicalDevice, VertexBufferView );
      if( !CreateStorageTexelBuffer( PhysicalDevice, *LogicalDevice, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof( particles[0] ) * particles.size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, false,
        *VertexBuffer, *VertexBufferMemory, *VertexBufferView ) ) {
        return false;
      }

      if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( particles[0] ) * particles.size(),
        &particles[0], *VertexBuffer, 0, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
        return false;
      }
    }

    // Staging buffer
    InitVkDestroyer( LogicalDevice, StagingBuffer );
    if( !CreateBuffer( *LogicalDevice, 2 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, *StagingBuffer ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, StagingBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *StagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, *StagingBufferMemory ) ) {
      return false;
    }

    // Uniform buffer
    InitVkDestroyer( LogicalDevice, UniformBuffer );
    InitVkDestroyer( LogicalDevice, UniformBufferMemory );
    if( !CreateUniformBuffer( PhysicalDevice, *LogicalDevice, 2 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      *UniformBuffer, *UniformBufferMemory ) ) {
      return false;
    }

    if( !UpdateStagingBuffer( true ) ) {
      return false;
    }

    // Descriptor sets with uniform buffer

    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings = {
      {
        0,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT |                // VkShaderStageFlags   stageFlags
        VK_SHADER_STAGE_GEOMETRY_BIT,
        nullptr                                     // const VkSampler    * pImmutableSamplers
      },
      {
        0,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,    // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_COMPUTE_BIT,                // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      }
    };

    DescriptorSetLayout.resize( 2 );
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout[0] );
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout[1] );
    if( !CreateDescriptorSetLayout( *LogicalDevice, { descriptor_set_layout_bindings[0] }, *DescriptorSetLayout[0] ) ) {
      return false;
    }
    if( !CreateDescriptorSetLayout( *LogicalDevice, { descriptor_set_layout_bindings[1] }, *DescriptorSetLayout[1] ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> descriptor_pool_sizes = {
      {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      },
      {
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,    // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      }
    };
    InitVkDestroyer( LogicalDevice, DescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 2, descriptor_pool_sizes, *DescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *DescriptorPool, { *DescriptorSetLayout[0], *DescriptorSetLayout[1] }, DescriptorSets ) ) {
      return false;
    }

    BufferDescriptorInfo buffer_descriptor_update = {
      DescriptorSets[0],                          // VkDescriptorSet                      TargetDescriptorSet
      0,                                          // uint32_t                             TargetDescriptorBinding
      0,                                          // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType                     TargetDescriptorType
      {                                           // std::vector<VkDescriptorBufferInfo>  BufferInfos
        {
          *UniformBuffer,                           // VkBuffer                             buffer
          0,                                        // VkDeviceSize                         offset
          VK_WHOLE_SIZE                             // VkDeviceSize                         range
        }
      }
    };

    TexelBufferDescriptorInfo storage_texel_buffer_descriptor_update = {
      DescriptorSets[1],                          // VkDescriptorSet                      TargetDescriptorSet
      0,                                          // uint32_t                             TargetDescriptorBinding
      0,                                          // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,    // VkDescriptorType                     TargetDescriptorType
      {                                           // std::vector<VkBufferView>            TexelBufferViews
        {
          *VertexBufferView
        }
      }
    };

    UpdateDescriptorSets( *LogicalDevice, {}, { buffer_descriptor_update }, { storage_texel_buffer_descriptor_update }, {} );

    // Render pass
    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,                     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                   // VkImageLayout                    finalLayout
      },
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        DepthFormat,                                      // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                    finalLayout
      }
    };

    VkAttachmentReference depth_attachment = {
      1,                                                // uint32_t                             attachment
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                        layout;
    };

    std::vector<SubpassParameters> subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
        {},                                           // std::vector<VkAttachmentReference>   InputAttachments
        {                                             // std::vector<VkAttachmentReference>   ColorAttachments
          {
            0,                                          // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                        layout
          }
        },
        {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
      &depth_attachment,                            // VkAttachmentReference const        * DepthStencilAttachment
      {}                                            // std::vector<uint32_t>                PreserveAttachments
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

    // Compute pipeline

    std::vector<unsigned char> compute_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/03 Drawing particles using compute and graphics pipelines/shader.comp.spv", compute_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> compute_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, compute_shader_spirv, *compute_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> compute_shader_stage_params = {
      {
        VK_SHADER_STAGE_COMPUTE_BIT,      // VkShaderStageFlagBits        ShaderStage
        *compute_shader_module,           // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> compute_shader_stage_create_infos;
    SpecifyPipelineShaderStages( compute_shader_stage_params, compute_shader_stage_create_infos );

    VkPushConstantRange push_constant_range = {
      VK_SHADER_STAGE_COMPUTE_BIT,    // VkShaderStageFlags     stageFlags
      0,                              // uint32_t               offset
      sizeof( float )                 // uint32_t               size
    };

    InitVkDestroyer( LogicalDevice, ComputePipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout[1] }, { push_constant_range }, *ComputePipelineLayout ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ComputePipeline );
    if( !CreateComputePipeline( *LogicalDevice, 0, compute_shader_stage_create_infos[0], *ComputePipelineLayout, VK_NULL_HANDLE, VK_NULL_HANDLE, *ComputePipeline ) ) {
      return false;
    }

    // Graphics pipeline

    std::vector<unsigned char> vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/03 Drawing particles using compute and graphics pipelines/shader.vert.spv", vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> vertex_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, vertex_shader_spirv, *vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> geometry_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/03 Drawing particles using compute and graphics pipelines/shader.geom.spv", geometry_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> geometry_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, geometry_shader_spirv, *geometry_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/12 Advanced Rendering Techniques/03 Drawing particles using compute and graphics pipelines/shader.frag.spv", fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer<VkShaderModule> fragment_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, fragment_shader_spirv, *fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *vertex_shader_module,            // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_GEOMETRY_BIT,     // VkShaderStageFlagBits        ShaderStage
        *geometry_shader_module,          // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *fragment_shader_module,          // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        8 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> vertex_attribute_descriptions = {
      {
        0,                              // uint32_t   location
        0,                              // uint32_t   binding
        VK_FORMAT_R32G32B32A32_SFLOAT,  // VkFormat   format
        0                               // uint32_t   offset
      },
      {
        1,                              // uint32_t   location
        0,                              // uint32_t   binding
        VK_FORMAT_R32G32B32A32_SFLOAT,  // VkFormat   format
        4 * sizeof( float )             // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( vertex_input_binding_descriptions, vertex_attribute_descriptions, vertex_input_state_create_info );

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( VK_PRIMITIVE_TOPOLOGY_POINT_LIST, false, input_assembly_state_create_info );

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
      {                     // std::vector<VkRect2D>     Scissors
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
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 1.0f, 0.0f, 1.0f, rasterization_state_create_info );

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info;
    SpecifyPipelineDepthAndStencilState( true, false, VK_COMPARE_OP_LESS_OR_EQUAL, false, 0.0f, 1.0f, false, {}, {}, depth_stencil_state_create_info );

    std::vector<VkPipelineColorBlendAttachmentState> attachment_blend_states = {
      {
        true,                                 // VkBool32                 blendEnable
        VK_BLEND_FACTOR_SRC_ALPHA,            // VkBlendFactor            srcColorBlendFactor
        VK_BLEND_FACTOR_ONE,                  // VkBlendFactor            dstColorBlendFactor
        VK_BLEND_OP_ADD,                      // VkBlendOp                colorBlendOp
        VK_BLEND_FACTOR_ONE,                  // VkBlendFactor            srcAlphaBlendFactor
        VK_BLEND_FACTOR_ONE,                  // VkBlendFactor            dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                      // VkBlendOp                alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT |            // VkColorComponentFlags    colorWriteMask
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

    InitVkDestroyer( LogicalDevice, GraphicsPipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout[0] }, {}, *GraphicsPipelineLayout ) ) {
      return false;
    }

    VkGraphicsPipelineCreateInfo pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, &depth_stencil_state_create_info, &blend_state_create_info,
      &dynamic_state_create_info, *GraphicsPipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, pipeline_create_info );

    std::vector<VkPipeline> graphics_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { pipeline_create_info }, VK_NULL_HANDLE, graphics_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, graphics_pipeline[0], GraphicsPipeline );

    InitVkDestroyer( LogicalDevice, ComputeSemaphore );
    if( !CreateSemaphore( *LogicalDevice, *ComputeSemaphore ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ComputeFence );
    if( !CreateFence( *LogicalDevice, true, *ComputeFence ) ) {
      return false;
    }

    return true;
  }

  virtual bool Draw() override {
    // Record command buffer with compute shader dispatch

    WaitSemaphoreInfo wait_semaphore_info = {
      *ComputeSemaphore,                  // VkSemaphore            Semaphore
      VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, // VkPipelineStageFlags   WaitingStage
    };

    if( !WaitForFences( *LogicalDevice, { *ComputeFence }, VK_FALSE, 2000000000 ) ) {
      return false;
    }

    if( !ResetFences( *LogicalDevice, { *ComputeFence } ) ) {
      return false;
    }

    if( !BeginCommandBufferRecordingOperation( ComputeCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
      return false;
    }

    BindDescriptorSets( ComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, *ComputePipelineLayout, 0, { DescriptorSets[1] }, {} );

    BindPipelineObject( ComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, *ComputePipeline );

    float time = TimerState.GetDeltaTime();
    ProvideDataToShadersThroughPushConstants( ComputeCommandBuffer, *ComputePipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof( float ), &time );

    DispatchComputeWork( ComputeCommandBuffer, PARTICLES_COUNT / 32 + 1, 1, 1 );

    if( !EndCommandBufferRecordingOperation( ComputeCommandBuffer ) ) {
      return false;
    }

    if( !SubmitCommandBuffersToQueue( ComputeQueue.Handle, {}, { ComputeCommandBuffer }, { *ComputeSemaphore }, *ComputeFence ) ) {
      return false;
    }

    // Prepare drawing function

    auto prepare_frame = [&]( VkCommandBuffer command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
      if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
        return false;
      }

      if( UpdateUniformBuffer ) {
        UpdateUniformBuffer = false;

        BufferTransition pre_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    CurrentAccess
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, { pre_transfer_transition } );

        std::vector<VkBufferCopy> regions = {
          {
            0,                          // VkDeviceSize     srcOffset
            0,                          // VkDeviceSize     dstOffset
            2 * 16 * sizeof( float )    // VkDeviceSize     size
          }
        };
        CopyDataBetweenBuffers( command_buffer, *StagingBuffer, *UniformBuffer, regions );

        BufferTransition post_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    CurrentAccess
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, { post_transfer_transition } );
      }

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_drawing = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
          PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
          GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
      }

      // Drawing
      BeginRenderPass( command_buffer, *RenderPass, framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f },{ 1.0f, 0 } }, VK_SUBPASS_CONTENTS_INLINE );

      VkViewport viewport = {
        0.0f,                                       // float    x
        0.0f,                                       // float    y
        static_cast<float>(Swapchain.Size.width),   // float    width
        static_cast<float>(Swapchain.Size.height),  // float    height
        0.0f,                                       // float    minDepth
        1.0f,                                       // float    maxDepth
      };
      SetViewportStateDynamically( command_buffer, 0, { viewport } );

      VkRect2D scissor = {
        {                                           // VkOffset2D     offset
          0,                                          // int32_t        x
          0                                           // int32_t        y
        },
        {                                           // VkExtent2D     extent
          Swapchain.Size.width,                       // uint32_t       width
          Swapchain.Size.height                       // uint32_t       height
        }
      };
      SetScissorStateDynamically( command_buffer, 0, { scissor } );

      BindVertexBuffers( command_buffer, 0, { { *VertexBuffer, 0 } } );

      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *GraphicsPipelineLayout, 0, { DescriptorSets[0] }, {} );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *GraphicsPipeline );

      DrawGeometry( command_buffer, PARTICLES_COUNT, 1, 0, 0 );

      EndRenderPass( command_buffer );

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_present = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        CurrentAccess
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
          GraphicsQueue.FamilyIndex,                // uint32_t             CurrentQueueFamily
          PresentQueue.FamilyIndex,                 // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
      }

      if( !EndCommandBufferRecordingOperation( command_buffer ) ) {
        return false;
      }
      return true;
    };

    return IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( *LogicalDevice, GraphicsQueue.Handle, PresentQueue.Handle,
      *Swapchain.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, *RenderPass, { wait_semaphore_info }, prepare_frame, FramesResources );
  }

  void OnMouseEvent() {
    UpdateStagingBuffer( false );
  }

  bool UpdateStagingBuffer( bool force ) {
    if( MouseState.Buttons[0].IsPressed ) {
      Camera.RotateHorizontally( 0.5f * MouseState.Position.Delta.X );
      Camera.RotateVertically( -0.5f * MouseState.Position.Delta.Y );
      force = true;
    }

    if( force ) {
      UpdateUniformBuffer = true;

      Matrix4x4 model_view_matrix = Camera.GetMatrix();

      if( !MapUpdateAndUnmapHostVisibleMemory( *LogicalDevice, *StagingBufferMemory, 0, sizeof( model_view_matrix[0] ) * model_view_matrix.size(), &model_view_matrix[0], true, nullptr ) ) {
        return false;
      }

      Matrix4x4 perspective_matrix = PreparePerspectiveProjectionMatrix( static_cast<float>(Swapchain.Size.width) / static_cast<float>(Swapchain.Size.height),
        50.0f, 0.5f, 10.0f );

      if( !MapUpdateAndUnmapHostVisibleMemory( *LogicalDevice, *StagingBufferMemory, sizeof( model_view_matrix[0] ) * model_view_matrix.size(),
        sizeof( perspective_matrix[0] ) * perspective_matrix.size(), &perspective_matrix[0], true, nullptr ) ) {
        return false;
      }
    }
    return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain() ) {
      return false;
    }

    if( IsReady() ) {
      if( !UpdateStagingBuffer( true ) ) {
        return false;
      }
    }
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "12/03 - Drawing particles using compute and graphics pipelines", 50, 25, 1280, 800, Sample )
