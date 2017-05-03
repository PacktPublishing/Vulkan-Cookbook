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
// 11-Drawing_Vertex_Normals

#include "CookbookSampleFramework.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  Mesh                                Model;
  VkDestroyer<VkBuffer>               VertexBuffer;
  VkDestroyer<VkDeviceMemory>         VertexBufferMemory;

  VkDestroyer<VkBuffer>               UniformBuffer;
  VkDestroyer<VkDeviceMemory>         UniformBufferMemory;

  VkDestroyer<VkDescriptorSetLayout>  DescriptorSetLayout;
  VkDestroyer<VkDescriptorPool>       DescriptorPool;
  std::vector<VkDescriptorSet>        DescriptorSets;

  VkDestroyer<VkRenderPass>           RenderPass;
  VkDestroyer<VkPipelineLayout>       PipelineLayout;
  VkDestroyer<VkPipeline>             ModelPipeline;
  VkDestroyer<VkPipeline>             NormalsPipeline;

  virtual bool Initialize( WindowParameters window_parameters ) override {
    VkPhysicalDeviceFeatures device_features = {};
    device_features.geometryShader = true;

    if( !InitializeVulkan( window_parameters, &device_features, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, false ) ) {
      return false;
    }

    // Vertex data
    if( !Load3DModelFromObjFile( "Data/Models/sphere.obj", true, false, false, true, Model ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, VertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof( Model.Data[0] ) * Model.Data.size(),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *VertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, VertexBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *VertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *VertexBufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( Model.Data[0] ) * Model.Data.size(),
      &Model.Data[0], *VertexBuffer, 0, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    // Uniform buffer
    InitVkDestroyer( LogicalDevice, UniformBuffer );
    InitVkDestroyer( LogicalDevice, UniformBufferMemory );
    if( !CreateUniformBuffer( PhysicalDevice, *LogicalDevice, 2 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      *UniformBuffer, *UniformBufferMemory ) ) {
      return false;
    }

    if( !UpdateUniformBuffer() ) {
      return false;
    }

    // Descriptor set with uniform buffer
    VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
      0,                                          // uint32_t             binding
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     descriptorType
      1,                                          // uint32_t             descriptorCount
      VK_SHADER_STAGE_VERTEX_BIT |                // VkShaderStageFlags   stageFlags
      VK_SHADER_STAGE_GEOMETRY_BIT,
      nullptr                                     // const VkSampler    * pImmutableSamplers
    };
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout );
    if( !CreateDescriptorSetLayout( *LogicalDevice, { descriptor_set_layout_binding }, *DescriptorSetLayout ) ) {
      return false;
    }

    VkDescriptorPoolSize descriptor_pool_size = {
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     type
      1                                           // uint32_t             descriptorCount
    };
    InitVkDestroyer( LogicalDevice, DescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 1, { descriptor_pool_size }, *DescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *DescriptorPool, { *DescriptorSetLayout }, DescriptorSets ) ) {
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

    UpdateDescriptorSets( *LogicalDevice, {}, { buffer_descriptor_update }, {}, {} );

    // Render pass
    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                          // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                           // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                      // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,               // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,            // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,           // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR             // VkImageLayout                    finalLayout
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

    // Model
    std::vector<unsigned char> vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/11 Drawing Vertex Normals/model.vert.spv", vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> vertex_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, vertex_shader_spirv, *vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/11 Drawing Vertex Normals/model.frag.spv", fragment_shader_spirv ) ) {
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
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *fragment_shader_module,          // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    // Normals
    std::vector<unsigned char> normals_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/11 Drawing Vertex Normals/normals.vert.spv", normals_vertex_shader_spirv ) ) {
      return false;
    }
    
    VkDestroyer<VkShaderModule> normals_vertex_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, normals_vertex_shader_spirv, *normals_vertex_shader_module ) ) {
      return false;
    }
    
    std::vector<unsigned char> normals_geometry_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/11 Drawing Vertex Normals/normals.geom.spv", normals_geometry_shader_spirv ) ) {
      return false;
    }
    
    VkDestroyer<VkShaderModule> normals_geometry_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, normals_geometry_shader_spirv, *normals_geometry_shader_module ) ) {
      return false;
    }
    
    std::vector<unsigned char> normals_fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/Other/11 Drawing Vertex Normals/normals.frag.spv", normals_fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer<VkShaderModule> normals_fragment_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, normals_fragment_shader_spirv, *normals_fragment_shader_module ) ) {
      return false;
    }
    
    std::vector<ShaderStageParameters> normals_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *normals_vertex_shader_module,    // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_GEOMETRY_BIT,     // VkShaderStageFlagBits        ShaderStage
        *normals_geometry_shader_module,  // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *normals_fragment_shader_module,  // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };
    
    std::vector<VkPipelineShaderStageCreateInfo> normals_shader_stage_create_infos;
    SpecifyPipelineShaderStages( normals_shader_stage_params, normals_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        6 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> vertex_attribute_descriptions = {
      {
        0,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        0                                                                         // uint32_t   offset
      },
      {
        1,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        3 * sizeof( float )                                                       // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( vertex_input_binding_descriptions, vertex_attribute_descriptions, vertex_input_state_create_info );

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false, input_assembly_state_create_info );

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
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, false, 0.0f, 1.0f, 0.0f, 1.0f, rasterization_state_create_info );

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
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout }, {}, *PipelineLayout ) ) {
      return false;
    }

    VkGraphicsPipelineCreateInfo model_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, nullptr, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, model_pipeline_create_info );

    std::vector<VkPipeline> model_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { model_pipeline_create_info }, VK_NULL_HANDLE, model_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, model_pipeline[0], ModelPipeline );

    VkGraphicsPipelineCreateInfo normals_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, normals_shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, nullptr, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *RenderPass, 0, VK_NULL_HANDLE, -1, normals_pipeline_create_info );

    std::vector<VkPipeline> normals_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { normals_pipeline_create_info }, VK_NULL_HANDLE, normals_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, normals_pipeline[0], NormalsPipeline );

    return true;
  }

  virtual bool Draw() override {
    auto prepare_frame = [&]( VkCommandBuffer command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
      if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
        return false;
      }

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_drawing = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
          PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
          GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
      }

      // Drawing
      BeginRenderPass( command_buffer, *RenderPass, framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f } }, VK_SUBPASS_CONTENTS_INLINE );

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

      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PipelineLayout, 0, DescriptorSets, {} );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *ModelPipeline );

      for( size_t i = 0; i < Model.Parts.size(); ++i ) {
        DrawGeometry( command_buffer, Model.Parts[i].VertexCount, 1, Model.Parts[i].VertexOffset, 0 );
      }

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *NormalsPipeline );

      for( size_t i = 0; i < Model.Parts.size(); ++i ) {
        DrawGeometry( command_buffer, Model.Parts[i].VertexCount, 1, Model.Parts[i].VertexOffset, 0 );
      }

      EndRenderPass( command_buffer );

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_present = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
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
      *Swapchain.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, *RenderPass, {}, prepare_frame, FramesResources );
  }

  bool UpdateUniformBuffer() {
    Matrix4x4 rotation_matrix = PrepareRotationMatrix( 40.0f, { 0.0f, -1.0f, 0.0f } );
    Matrix4x4 translation_matrix = PrepareTranslationMatrix( 0.0f, 0.0f, -3.0f );
    Matrix4x4 model_view_matrix = translation_matrix * rotation_matrix;
    Matrix4x4 perspective_matrix = PreparePerspectiveProjectionMatrix( static_cast<float>(Swapchain.Size.width) / static_cast<float>(Swapchain.Size.height),
      50.0f, 1.0f, 2.6f );

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( model_view_matrix[0] ) * model_view_matrix.size(),
      &model_view_matrix[0], *UniformBuffer, 0, 0, VK_ACCESS_UNIFORM_READ_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( perspective_matrix[0] ) * perspective_matrix.size(),
      &perspective_matrix[0], *UniformBuffer, 16 * sizeof( float ), 0, VK_ACCESS_UNIFORM_READ_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FramesResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, false ) ) {
      return false;
    }

    if( !UpdateUniformBuffer() ) {
      return false;
    }
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "11 - Drawing Vertex Normals", 50, 25, 1280, 800, Sample )
